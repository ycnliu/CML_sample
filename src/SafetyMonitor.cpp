#include "SafetyMonitor.h"

SafetyMonitor::SafetyMonitor(int axesCount) : emergencyStopEngaged(false) {
    if (axesCount < 1) axesCount = 1;
    minBounds.resize(axesCount);
    maxBounds.resize(axesCount);
    // Default bounds: very wide range (user can tighten via setAxisBounds)
    for (int i = 0; i < axesCount; ++i) {
        minBounds[i] = -1e6;
        maxBounds[i] =  1e6;
    }
}

void SafetyMonitor::setAxisBounds(int axisIndex, double minPos, double maxPos) {
    if (axisIndex < 0 || axisIndex >= (int)minBounds.size()) return;
    minBounds[axisIndex] = minPos;
    maxBounds[axisIndex] = maxPos;
}

bool SafetyMonitor::checkPosition(const std::vector<double>& positions) const {
    if (emergencyStopEngaged) {
        // If emergency stop is active, treat any move as unsafe
        return false;
    }
    int n = positions.size();
    int maxAxis = (int)minBounds.size();
    for (int i = 0; i < n && i < maxAxis; ++i) {
        if (positions[i] < minBounds[i] || positions[i] > maxBounds[i]) {
            return false;
        }
    }
    return true;
}

void SafetyMonitor::triggerEStop() {
    emergencyStopEngaged = true;
}

void SafetyMonitor::clearEStop() {
    emergencyStopEngaged = false;
}

bool SafetyMonitor::isEmergencyStop() const {
    return emergencyStopEngaged;
}
