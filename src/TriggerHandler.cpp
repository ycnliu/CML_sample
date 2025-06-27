#include "TriggerHandler.h"

TriggerHandler::TriggerHandler() {
    // Initialize known triggers to false (inactive)
    triggerStates[TRIG_START]   = false;
    triggerStates[TRIG_STOP]    = false;
    triggerStates[TRIG_CAPTURE] = false;
}

void TriggerHandler::setTrigger(int triggerId, bool state) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        triggerStates[triggerId] = state;
    }
    // Notify waiting threads if setting trigger to active
    if (state) {
        cv.notify_all();
    }
}

bool TriggerHandler::isTriggered(int triggerId) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = triggerStates.find(triggerId);
    if (it != triggerStates.end()) {
        return it->second;
    }
    return false;
}

void TriggerHandler::waitForTrigger(int triggerId) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&]{ return triggerStates[triggerId]; });
    // (Trigger remains in active state until cleared by clearTrigger)
}

void TriggerHandler::clearTrigger(int triggerId) {
    std::lock_guard<std::mutex> lock(mtx);
    triggerStates[triggerId] = false;
}
