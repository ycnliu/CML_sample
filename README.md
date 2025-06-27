# 🧠 CML Optical Inspection Controller

A modular C++ project simulating a multi-axis optical inspection system using a mock CML (Copley Motion Library) API. This demonstrates real-world design patterns with clear modular separation and testability.

---

## 📁 Project Structure

```
sample_proj/
├── CMakeLists.txt
├── include/
│   └── cml.h                     # Simulated CML interface
├── src/
│   ├── InspectionController.cpp  # Main entry point
│   ├── MotionController.*        # Motion logic
│   ├── CalibrationManager.*      # Calibration math
│   ├── TriggerHandler.*          # Trigger I/O processing
│   ├── SafetyMonitor.*           # Limit checking
│   └── Logger.*                  # File logging
├── tests/
│   ├── main.cpp
│   ├── test_MotionController.cpp
│   ├── test_CalibrationManager.cpp
│   ├── test_TriggerHandler.cpp
│   ├── test_SafetyMonitor.cpp
│   └── test_Logger.cpp
```

---

## 🛠️ Build Instructions

> Ensure you have CMake ≥ 3.14 and a C++17-compatible compiler.

```bash
# From project root
mkdir build && cd build
cmake ..
make
```

This builds:
- `InspectionController`: the main inspection runtime
- `run_tests`: unit tests (using Catch2)

---

## 🚀 Run the System

```bash
./InspectionController
```

This simulates 8 inspection cycles, with position calibration, motion execution, I/O trigger checking, and safety monitoring.

---

## ✅ Run Tests

```bash
./run_tests
```

Unit tests are built on [Catch2 v3](https://github.com/catchorg/Catch2), fetched automatically via CMake.

---

## 🧪 CMakeLists.txt Highlights

```cmake
# Add the main controller
add_executable(InspectionController
  src/InspectionController.cpp
  src/MotionController.cpp
  src/CalibrationManager.cpp
  src/TriggerHandler.cpp
  src/SafetyMonitor.cpp
  src/Logger.cpp
)
target_include_directories(InspectionController PRIVATE include)

# Add Catch2 for tests
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)

# Unit test runner
add_executable(run_tests
  tests/main.cpp
  tests/test_*.cpp
)
target_link_libraries(run_tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(run_tests)
```

---

## 🔧 Notes

- `include/cml.h` is a mock SDK; replace it with the real SDK for hardware integration.
- Modify `IP_ADDRESS` in `InspectionController.cpp` if not using `eth0`.
- All motion commands and inputs are simulated unless integrated with hardware.

---

## 👤 Author

**Yuchen Liu**
[LinkedIn](https://www.linkedin.com/in/ycnliu/) | [Email](mailto:ycnliu@ucdavis.edu)

---
```
