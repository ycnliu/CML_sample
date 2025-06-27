#include "catch.hpp"
#include "CalibrationManager.h"

TEST_CASE("CalibrationManager default (identity) transform", "[CalibrationManager]") {
    CalibrationManager calib;
    // Identity should leave coordinates unchanged
    std::vector<double> input = { 10.0, -5.0 };
    std::vector<double> output = calib.applyCalibration(input);
    REQUIRE(output.size() == 2);
    REQUIRE(output[0] == Approx(10.0));
    REQUIRE(output[1] == Approx(-5.0));
    // Identity with extra coordinates (e.g., Z) should not alter them
    input = { 3.0, 4.0, 7.5 };
    output = calib.applyCalibration(input);
    REQUIRE(output.size() == 3);
    REQUIRE(output[0] == Approx(3.0));
    REQUIRE(output[1] == Approx(4.0));
    REQUIRE(output[2] == Approx(7.5));
}

TEST_CASE("CalibrationManager custom transform (scale/offset/rotation)", "[CalibrationManager]") {
    CalibrationManager calib;
    // Scaling matrix: scale X and Y by 2
    std::array<double, 9> scaleMat = {2, 0, 0,
                                      0, 2, 0,
                                      0, 0, 1};
    calib.setCalibrationMatrix(scaleMat);
    auto out = calib.applyCalibration({1.0, 1.0});
    REQUIRE(out[0] == Approx(2.0));
    REQUIRE(out[1] == Approx(2.0));
    // Offset matrix: x' = x + 5, y' = y + 10
    std::array<double, 9> offsetMat = {1, 0, 5,
                                       0, 1, 10,
                                       0, 0, 1};
    calib.setCalibrationMatrix(offsetMat);
    out = calib.applyCalibration({0.0, 0.0});
    REQUIRE(out[0] == Approx(5.0));
    REQUIRE(out[1] == Approx(10.0));
    // 90-degree rotation matrix (about origin): x' = -y, y' = x
    std::array<double, 9> rot90 = {0, -1, 0,
                                   1,  0, 0,
                                   0,  0, 1};
    calib.setCalibrationMatrix(rot90);
    out = calib.applyCalibration({ 2.0, 0.0 });
    // (2,0) rotated 90 deg should result in (0,2) 
    REQUIRE(out[0] == Approx(0.0));
    REQUIRE(out[1] == Approx(2.0));
}
