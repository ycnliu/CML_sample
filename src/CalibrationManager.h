#ifndef CALIBRATION_MANAGER_H
#define CALIBRATION_MANAGER_H

#include <vector>
#include <array>

// Manages calibration transforms (e.g., coordinate alignment, scaling, offsets)
class CalibrationManager {
private:
    // 3x3 homogeneous transformation matrix for 2D (X, Y) coordinates
    std::array<double, 9> calibMatrix;
public:
    CalibrationManager();
    // Set the calibration matrix (array of 9 values representing 3x3 matrix)
    void setCalibrationMatrix(const std::array<double, 9>& matrix);
    // Apply calibration to input coordinates (only X and Y are transformed; additional coordinates pass through)
    std::vector<double> applyCalibration(const std::vector<double>& coordinates) const;
};

#endif // CALIBRATION_MANAGER_H
