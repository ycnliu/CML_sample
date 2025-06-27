#ifndef MOTION_CONTROLLER_H
#define MOTION_CONTROLLER_H

#include <vector>
#include "cml.h"

// Forward declarations of component classes
class CalibrationManager;
class TriggerHandler;
class SafetyMonitor;
class Logger;

// High-level motion controller coordinating motors, calibration, triggers, and safety
class MotionController {
public:
    // System state for tracking overall controller status
    enum class State { IDLE, MOVING, ERROR, EMERGENCY_STOP };
private:
    CML::Network network;               // Network interface (simulated hardware connection)
    std::vector<CML::Amp> axes;         // Controlled motor axes
    int axesCount;
    bool initialized;
    State currentState;
    // References to external components
    CalibrationManager& calibManager;
    TriggerHandler& triggerHandler;
    SafetyMonitor& safetyMonitor;
    Logger& logger;
public:
    MotionController(CalibrationManager& calib, TriggerHandler& trigger,
                     SafetyMonitor& safety, Logger& log, int numAxes = 1);
    // Initialize network and all axes
    const CML::Error* initialize();
    // Move to target positions (size of vector must equal number of axes).
    // If calibrated==true, interpret targetPositions in world coordinates and apply calibration.
    const CML::Error* homeAll();
    const CML::Error* moveTo(const std::vector<double>& targetPositions, bool calibrated = true);
    // Perform an emergency stop on all axes and mark system as halted
    void emergencyStop();
    // Get current controller state
    State getState() const;
    // Get the current position of a specified axis
    double getAxisPosition(int axisIndex) const;
};

#endif // MOTION_CONTROLLER_H
