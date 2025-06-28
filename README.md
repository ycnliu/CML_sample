# CML Sample for Motor Controller with JNI and Docker integration

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
├── cpp_wrapper/              # C++ JNI wrapper and native dylib
│   ├── MotionSystemWrapper.cpp
│   ├── libmotionwrapper.dylib
│   └── com/cml/wrapper/MotionSystemWrapper.java
├── spring-motion-api/       # Spring Boot REST API
│   ├── src/
│   ├── pom.xml
│   ├── Dockerfile
│   └── target/motion-api-1.0.0.jar
└── README.md
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

##  Run the System

```bash
./InspectionController
```

This simulates 8 inspection cycles, with position calibration, motion execution, I/O trigger checking, and safety monitoring.

---

##  Run Tests

```bash
./run_tests
```

Unit tests are built on [Catch2 v3](https://github.com/catchorg/Catch2), fetched automatically via CMake.

---


-------------------------------
Native C++ JNI Wrapper
-------------------------------
1. Navigate to cpp_wrapper:
   $ cd cpp_wrapper

2. Compile using your Makefile:
   $ make

3. Ensure that libmotionwrapper.dylib is generated.

-------------------------------
Spring Boot REST API
-------------------------------
1. Build the JAR:
   $ cd spring-motion-api
   $ mvn clean install

2. Run with JNI library path:
   $ mvn spring-boot:run -Dspring-boot.run.jvmArguments="-Djava.library.path=../cpp_wrapper"

-------------------------------
Docker Deployment
-------------------------------
1. Ensure motion-api-1.0.0.jar exists in target/

2. Build Docker image:
   $ docker build -t spring-motion-api .

3. Run container (exposing port 8080):
   $ docker run -p 8080:8080 spring-motion-api

-------------------------------
API Endpoints
-------------------------------
POST /motion/initialize     -> Initialize all axes
POST /motion/home           -> Home all axes
POST /motion/move           -> Move to positions

Example:
   $ curl -X POST http://localhost:8080/motion/home

To move axes:
   $ curl -X POST -H "Content-Type: application/json" \
     -d '[0.0, 50.0, 100.0]' http://localhost:8080/motion/move

-------------------------------
Requirements
-------------------------------
- Java 17+
- C++ compiler
- Maven
- Docker
- JNI-compatible OS (e.g., macOS for .dylib)

-------------------------------
License
-------------------------------
MIT License
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
