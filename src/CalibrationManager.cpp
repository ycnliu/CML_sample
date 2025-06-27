#include "CalibrationManager.h"
#include <cmath>

CalibrationManager::CalibrationManager() {
    // Initialize to identity matrix (no transformation)
    calibMatrix = {1.0, 0.0, 0.0,
                   0.0, 1.0, 0.0,
                   0.0, 0.0, 1.0};
}

void CalibrationManager::setCalibrationMatrix(const std::array<double, 9>& matrix) {
    calibMatrix = matrix;
}

std::vector<double> CalibrationManager::applyCalibration(const std::vector<double>& coordinates) const {
    std::vector<double> result = coordinates;
    if (coordinates.size() >= 2) {
        // Apply 2D homogeneous transform: [x', y', w'] = [x, y, 1] * calibMatrix
        double x = coordinates[0];
        double y = coordinates[1];
        double x_prime = x * calibMatrix[0] + y * calibMatrix[1] + 1.0 * calibMatrix[2];
        double y_prime = x * calibMatrix[3] + y * calibMatrix[4] + 1.0 * calibMatrix[5];
        double w_prime = x * calibMatrix[6] + y * calibMatrix[7] + 1.0 * calibMatrix[8];
        if (w_prime != 0.0) {
            x_prime /= w_prime;
            y_prime /= w_prime;
        }
        result[0] = x_prime;
        result[1] = y_prime;
    }
    // Any additional coordinates (e.g., Z or Theta) remain unchanged in result
    return result;
}
