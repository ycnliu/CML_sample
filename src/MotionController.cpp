#include "MotionController.h"
#include "CalibrationManager.h"
#include "TriggerHandler.h"
#include "SafetyMonitor.h"
#include "Logger.h"
#include <string>

MotionController::MotionController(CalibrationManager& calib, TriggerHandler& trigger,
                                   SafetyMonitor& safety, Logger& log, int numAxes)
    : axesCount(numAxes), initialized(false), currentState(State::IDLE),
      calibManager(calib), triggerHandler(trigger), safetyMonitor(safety), logger(log) {
    if (axesCount < 1) axesCount = 1;
    axes.resize(axesCount);
}

const CML::Error* MotionController::initialize() {
    // Open the network connection
    const CML::Error* err = network.Open();
    if (err != CML::SUCCESS) {
        logger.log("Error: Failed to open network");
        currentState = State::ERROR;
        return err;
    }
    // Initialize the first axis (primary axis)
    CML::AmpSettings settings;
    err = axes[0].Init(network, -1, settings);
    if (err != CML::SUCCESS) {
        logger.log("Error: Failed to initialize primary axis");
        currentState = State::ERROR;
        return err;
    }
    logger.log("Primary axis initialized");
    // Initialize additional axes (if any)
    for (int i = 1; i < axesCount; ++i) {
        err = axes[i].InitSubAxis(axes[0], i+1, settings);
        if (err != CML::SUCCESS) {
            logger.log(std::string("Error: Failed to initialize axis ") + std::to_string(i+1));
            currentState = State::ERROR;
            return err;
        }
        logger.log(std::string("Axis ") + std::to_string(i+1) + " initialized");
    }
    initialized = true;
    currentState = State::IDLE;
    logger.log("MotionController initialization complete");
    return CML::SUCCESS;
}
const CML::Error* MotionController::homeAll() {
  if (!initialized) {
      logger.log("Home failed: MotionController not initialized");
      currentState = State::ERROR;
      static CML::Error errNotInit(-110, "MotionController not initialized");
      return &errNotInit;
  }

  logger.log("Starting homing sequence for all axes...");

  CML::HomeConfig homeCfg;
  homeCfg.method = CML::CHM_NONE; // Adjust to CHM_INDEX_POS or others if needed
  homeCfg.offset = 0;
  homeCfg.velFast = 10000;
  homeCfg.velSlow = 1000;

  for (int i = 0; i < axesCount; ++i) {
      const CML::Error* err = axes[i].GoHome(homeCfg);
      if (err != CML::SUCCESS) {
          logger.log("Error: Failed to home axis " + std::to_string(i+1));
          currentState = State::ERROR;
          return err;
      }
  }

  const CML::Error* waitErr = CML::Amp::WaitMoveDone(axes.data(), axesCount, 20000);
  if (waitErr != CML::SUCCESS) {
      logger.log("Error: Timeout or failure during homing wait");
      currentState = State::ERROR;
      return waitErr;
  }

  currentState = State::IDLE;
  logger.log("Homing complete on all axes.");
  return CML::SUCCESS;
}
const CML::Error* MotionController::moveTo(const std::vector<double>& targetPositions, bool calibrated) {
    if (!initialized) {
        logger.log("Move failed: MotionController not initialized");
        currentState = State::ERROR;
        static CML::Error errNotInit(-100, "MotionController not initialized");
        return &errNotInit;
    }
    if (safetyMonitor.isEmergencyStop()) {
        logger.log("Move aborted: Emergency Stop is active");
        currentState = State::EMERGENCY_STOP;
        static CML::Error errEStop(-101, "Emergency stop active");
        return &errEStop;
    }
    if ((int)targetPositions.size() != axesCount) {
        logger.log("Move failed: Target position vector size mismatch");
        currentState = State::ERROR;
        static CML::Error errSize(-102, "Incorrect number of target positions");
        return &errSize;
    }
    // Apply calibration if coordinates are in world frame
    std::vector<double> stagePositions = targetPositions;
    if (calibrated) {
        stagePositions = calibManager.applyCalibration(targetPositions);
        if (axesCount >= 2) {
            logger.log("Applied calibration transform: [" +
                       std::to_string(targetPositions[0]) + "," + std::to_string(targetPositions[1]) + "] -> [" +
                       std::to_string(stagePositions[0]) + "," + std::to_string(stagePositions[1]) + "]");
        } else {
            logger.log("Applied calibration transform to target positions");
        }
    }
    // Check safety limits for each axis
    if (!safetyMonitor.checkPosition(stagePositions)) {
        logger.log("Move denied: Target position out of safety bounds");
        currentState = State::ERROR;
        static CML::Error errBounds(-103, "Target position out of safety bounds");
        return &errBounds;
    }
    // Execute move on all axes
    logger.log("Moving to positions: [" +
        (axesCount > 0 ? std::to_string(stagePositions[0]) : "") +
        (axesCount > 1 ? "," + std::to_string(stagePositions[1]) : "") +
        (axesCount > 2 ? "," + std::to_string(stagePositions[2]) : "") +
        (axesCount > 3 ? "," + std::to_string(stagePositions[3]) : "") + "]");
    currentState = State::MOVING;
    for (int i = 0; i < axesCount; ++i) {
        const CML::Error* moveErr = axes[i].MoveAbs(stagePositions[i]);
        if (moveErr != CML::SUCCESS) {
            logger.log(std::string("Error moving axis ") + std::to_string(i+1) +
                       " to position " + std::to_string(stagePositions[i]));
            currentState = State::ERROR;
            return moveErr;
        }
    }
    // In a real system, we might wait for motion completion or check status here
    currentState = State::IDLE;
    logger.log("Move completed");
    return CML::SUCCESS;
}

void MotionController::emergencyStop() {
    // Stop all axes immediately
    for (int i = 0; i < axesCount; ++i) {
        axes[i].Stop();
    }
    // Engage safety lockout and update state
    safetyMonitor.triggerEStop();
    currentState = State::EMERGENCY_STOP;
    logger.log("Emergency Stop engaged! All motion halted.");
}

MotionController::State MotionController::getState() const {
    return currentState;
}

double MotionController::getAxisPosition(int axisIndex) const {
    if (axisIndex < 0 || axisIndex >= axesCount) {
        return 0.0;
    }
    return axes[axisIndex].GetPosition();
}
