#include "catch.hpp"
#include "TriggerHandler.h"
#include <thread>
#include <chrono>

TEST_CASE("TriggerHandler initial state and set/clear", "[TriggerHandler]") {
    TriggerHandler triggers;
    // All known triggers should start as false
    REQUIRE(triggers.isTriggered(TRIG_START) == false);
    REQUIRE(triggers.isTriggered(TRIG_STOP) == false);
    REQUIRE(triggers.isTriggered(TRIG_CAPTURE) == false);
    // Set a trigger and check state
    triggers.setTrigger(TRIG_START, true);
    REQUIRE(triggers.isTriggered(TRIG_START) == true);
    // Clear trigger and check again
    triggers.clearTrigger(TRIG_START);
    REQUIRE(triggers.isTriggered(TRIG_START) == false);
}

TEST_CASE("TriggerHandler waitForTrigger unblocks on trigger", "[TriggerHandler]") {
    TriggerHandler triggers;
    bool signaled = false;
    // Start a thread that will trigger after a short delay
    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        triggers.setTrigger(TRIG_CAPTURE, true);
    });
    // Wait on the trigger (should unblock when thread sets it)
    triggers.waitForTrigger(TRIG_CAPTURE);
    signaled = true;
    t.join();
    // After waitForTrigger returns, the trigger should be active
    REQUIRE(signaled == true);
    REQUIRE(triggers.isTriggered(TRIG_CAPTURE) == true);
}
