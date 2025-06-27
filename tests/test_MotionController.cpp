#include "catch.hpp"
#include "MotionController.h"
#include "CalibrationManager.h"
#include "TriggerHandler.h"
#include "SafetyMonitor.h"
#include "Logger.h"

TEST_CASE("MotionController initialization and state", "[MotionController]") {
    CalibrationManager calib;
    TriggerHandler triggers;
    SafetyMonitor safety(2);
    Logger logger;
    logger.clear();
    // Set safety bounds for two axes
    safety.setAxisBounds(0, 0.0, 100.0);
    safety.setAxisBounds(1, 0.0, 100.0);
    MotionController ctrl(calib, triggers, safety, logger, 2);
    // Before initialization
    REQUIRE(ctrl.getState() == MotionController::State::IDLE);
    // Initialize the controller
    const CML::Error* err = ctrl.initialize();
    REQUIRE(err == CML::SUCCESS);
    REQUIRE(ctrl.getState() == MotionController::State::IDLE);
    // Logged messages should include initialization details
    auto logs = logger.getLogs();
    REQUIRE_FALSE(logs.empty());
    REQUIRE(logs.back() == std::string("MotionController initialization complete"));
}

TEST_CASE("MotionController move within bounds", "[MotionController]") {
    CalibrationManager calib;
    TriggerHandler triggers;
    SafetyMonitor safety(1);
    Logger logger;
    logger.clear();
    // Set safety bounds for single axis
    safety.setAxisBounds(0, -50.0, 50.0);
    MotionController ctrl(calib, triggers, safety, logger, 1);
    ctrl.initialize();
    // Command a move within safe bounds
    const CML::Error* err = ctrl.moveTo({ 25.0 });
    REQUIRE(err == CML::SUCCESS);
    REQUIRE(ctrl.getState() == MotionController::State::IDLE);
    // Axis position should be updated
    REQUIRE(ctrl.getAxisPosition(0) == Approx(25.0));
    // Check that a log entry for move completion exists
    auto logs = logger.getLogs();
    REQUIRE(std::find(logs.begin(), logs.end(), std::string("Move completed")) != logs.end());
}

TEST_CASE("MotionController safety bounds enforcement", "[MotionController]") {
    CalibrationManager calib;
    TriggerHandler triggers;
    SafetyMonitor safety(2);
    Logger logger;
    logger.clear();
    // Set tight bounds for two axes
    safety.setAxisBounds(0, 0.0, 10.0);
    safety.setAxisBounds(1, 0.0, 10.0);
    MotionController ctrl(calib, triggers, safety, logger, 2);
    ctrl.initialize();
    // Attempt a move outside bounds
    const CML::Error* err = ctrl.moveTo({ 5.0, 15.0 }); // Y=15 exceeds max 10
    REQUIRE(err != CML::SUCCESS);
    REQUIRE(ctrl.getState() == MotionController::State::ERROR);
    // Position should not have changed (remains at initial 0)
    REQUIRE(ctrl.getAxisPosition(0) == Approx(0.0));
    REQUIRE(ctrl.getAxisPosition(1) == Approx(0.0));
    // Log should indicate safety bounds violation
    auto logs = logger.getLogs();
    REQUIRE(std::find(logs.begin(), logs.end(),
            std::string("Move denied: Target position out of safety bounds")) != logs.end());
}

TEST_CASE("MotionController emergency stop handling", "[MotionController]") {
    CalibrationManager calib;
    TriggerHandler triggers;
    SafetyMonitor safety(1);
    Logger logger;
    logger.clear();
    safety.setAxisBounds(0, 0.0, 100.0);
    MotionController ctrl(calib, triggers, safety, logger, 1);
    ctrl.initialize();
    // Engage emergency stop
    ctrl.emergencyStop();
    REQUIRE(ctrl.getState() == MotionController::State::EMERGENCY_STOP);
    // Further moves should be blocked
    const CML::Error* err = ctrl.moveTo({ 50.0 });
    REQUIRE(err != CML::SUCCESS);
    REQUIRE(ctrl.getState() == MotionController::State::EMERGENCY_STOP);
    // Log should have emergency stop message
    auto logs = logger.getLogs();
    REQUIRE(std::find_if(logs.begin(), logs.end(), [](const std::string& msg) {
                return msg.find("Emergency Stop engaged") != std::string::npos;
            }) != logs.end());
}

TEST_CASE("MotionController calibration application", "[MotionController]") {
    CalibrationManager calib;
    TriggerHandler triggers;
    SafetyMonitor safety(2);
    Logger logger;
    logger.clear();
    // Set calibration to include an offset: x' = x + 5, y' = y + 0
    std::array<double, 9> matrix = {1, 0, 5,
                                    0, 1, 0,
                                    0, 0, 1};
    calib.setCalibrationMatrix(matrix);
    // Set wide bounds to not interfere
    safety.setAxisBounds(0, -100.0, 100.0);
    safety.setAxisBounds(1, -100.0, 100.0);
    MotionController ctrl(calib, triggers, safety, logger, 2);
    ctrl.initialize();
    // Move to (0,0) in world coordinates, expect stage to move to (5,0) due to calibration
    const CML::Error* err = ctrl.moveTo({ 0.0, 0.0 }, true);
    REQUIRE(err == CML::SUCCESS);
    REQUIRE(ctrl.getAxisPosition(0) == Approx(5.0));
    REQUIRE(ctrl.getAxisPosition(1) == Approx(0.0));
    // Calibration log entry should be present
    auto logs = logger.getLogs();
    REQUIRE(std::any_of(logs.begin(), logs.end(), [](const std::string& m){
                return m.find("Applied calibration transform") != std::string::npos;
            }));
}
