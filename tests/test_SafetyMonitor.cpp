#include "catch.hpp"
#include "SafetyMonitor.h"

TEST_CASE("SafetyMonitor default bounds and basic checks", "[SafetyMonitor]") {
    SafetyMonitor safety(1);
    // Default bounds are wide, so typical positions should be allowed
    REQUIRE(safety.checkPosition({0.0}) == true);
    REQUIRE(safety.checkPosition({500000.0}) == true);   // within default 1e6
    REQUIRE(safety.checkPosition({1000000.0}) == true);  // exactly at upper bound
    REQUIRE(safety.checkPosition({1000001.0}) == false); // just above upper bound
}

TEST_CASE("SafetyMonitor custom bounds enforcement", "[SafetyMonitor]") {
    SafetyMonitor safety(2);
    safety.setAxisBounds(0, -10.0, 10.0);
    safety.setAxisBounds(1, 0.0, 50.0);
    // Values within bounds
    REQUIRE(safety.checkPosition({0.0, 25.0}) == true);
    REQUIRE(safety.checkPosition({-10.0, 0.0}) == true);
    REQUIRE(safety.checkPosition({10.0, 50.0}) == true);
    // Out of bounds on axis 0 (below min)
    REQUIRE(safety.checkPosition({-11.0, 10.0}) == false);
    // Out of bounds on axis 0 (above max)
    REQUIRE(safety.checkPosition({11.0, 10.0}) == false);
    // Out of bounds on axis 1 (above max)
    REQUIRE(safety.checkPosition({0.0, 60.0}) == false);
}

TEST_CASE("SafetyMonitor emergency stop behavior", "[SafetyMonitor]") {
    SafetyMonitor safety(1);
    safety.setAxisBounds(0, 0.0, 100.0);
    // Initially, checkPosition should be true for in-bound position
    REQUIRE(safety.checkPosition({50.0}) == true);
    // Engage emergency stop
    safety.triggerEStop();
    // Now all moves should be considered unsafe
    REQUIRE(safety.isEmergencyStop() == true);
    REQUIRE(safety.checkPosition({50.0}) == false);
    REQUIRE(safety.checkPosition({0.0}) == false);
    // Clear emergency stop condition
    safety.clearEStop();
    REQUIRE(safety.isEmergencyStop() == false);
    // Safety check should revert to normal bounds checking
    REQUIRE(safety.checkPosition({50.0}) == true);
    REQUIRE(safety.checkPosition({-1.0}) == false); // still out of bounds (below min 0)
}
