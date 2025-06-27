#ifndef SAFETY_MONITOR_H
#define SAFETY_MONITOR_H

#include <vector>

// Monitors and enforces safety limits (position bounds, emergency stop state)
class SafetyMonitor {
private:
    std::vector<double> minBounds;
    std::vector<double> maxBounds;
    bool emergencyStopEngaged;
public:
    SafetyMonitor(int axesCount = 1);
    // Define allowed position range for a specific axis
    void setAxisBounds(int axisIndex, double minPos, double maxPos);
    // Check if given positions are within bounds (returns false if any axis out of range or if E-stop engaged)
    bool checkPosition(const std::vector<double>& positions) const;
    // Trigger an emergency stop condition (engage E-stop)
    void triggerEStop();
    // Clear the emergency stop condition (for recovery procedures)
    void clearEStop();
    // Query whether emergency stop is currently engaged
    bool isEmergencyStop() const;
};

#endif // SAFETY_MONITOR_H
