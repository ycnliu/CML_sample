#ifndef TRIGGER_HANDLER_H
#define TRIGGER_HANDLER_H

#include <mutex>
#include <condition_variable>
#include <unordered_map>

// Handles digital I/O triggers (e.g., sensor inputs or trigger signals)
class TriggerHandler {
private:
    std::unordered_map<int, bool> triggerStates;
    std::mutex mtx;
    std::condition_variable cv;
public:
    TriggerHandler();
    // Manually set a trigger state (simulating an external signal)
    void setTrigger(int triggerId, bool state);
    // Check if a trigger is currently active
    bool isTriggered(int triggerId);
    // Block until the specified trigger becomes active (one-time wait)
    void waitForTrigger(int triggerId);
    // Clear the specified trigger (set it to inactive/false)
    void clearTrigger(int triggerId);
};

// Example trigger ID definitions (can be expanded as needed)
static const int TRIG_START   = 1;
static const int TRIG_STOP    = 2;
static const int TRIG_CAPTURE = 3;

#endif // TRIGGER_HANDLER_H
