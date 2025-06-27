cmake_minimum_required(VERSION 3.14)
project(InspectionControllerProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2 # Or latest stable tag
)
FetchContent_MakeAvailable(Catch2)

# Include headers
include_directories(include)
# Find JNI headers
include_directories(
    /Library/Java/JavaVirtualMachines/openjdk.jdk/Contents/Home/include
    /Library/Java/JavaVirtualMachines/openjdk.jdk/Contents/Home/include/darwin
)

# Add source files
add_library(InspectionCore
    src/MotionController.cpp
    src/CalibrationManager.cpp
    src/TriggerHandler.cpp
    src/SafetyMonitor.cpp
    src/Logger.cpp
)

target_include_directories(InspectionCore PUBLIC include src)

# Add test executable
add_executable(run_tests
    tests/main.cpp
    tests/test_MotionController.cpp
    tests/test_CalibrationManager.cpp
    tests/test_TriggerHandler.cpp
    tests/test_SafetyMonitor.cpp
    tests/test_Logger.cpp
)

target_link_libraries(run_tests PRIVATE InspectionCore Catch2::Catch2WithMain)

