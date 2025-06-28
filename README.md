# CML Sample for Motor Controller with JNI and Docker integration

A modular C++ project simulating a multi-axis optical inspection system using a mock CML (Copley Motion Library) API. This demonstrates real-world design patterns with clear modular separation and testability.

---

## 📁 Project Structure

```
├── include/                     # C++ headers
├── src/                         # C++ implementations
├── cpp_wrapper/                     # Native C++ logic + JNI interface
│   ├── MotionSystemWrapper.cpp     # JNI C++ bridge
│   ├── libmotionwrapper.so         # Linux shared object
│   ├── libmotionwrapper.dylib      # (Optional) macOS dynamic library
│   ├── Makefile                    # Build rules for JNI library
│   └── CMakeLists.txt              # (Optional) for CMake builds

├── java_wrapper/                   # Pure Java JNI wrapper
│   ├── com/cml/wrapper/            # Package structure
│   │   └── MotionSystemWrapper.java
│   ├── TestJNI.java                # Test entry point
│   └── build/                      # Compiled .class files

├── spring-motion-api/             # Spring Boot REST API
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/com/example/motion/
│   │   │   │   ├── controller/MotionController.java
│   │   │   │   ├── service/MotionService.java
│   │   │   │   └── MotionApiApplication.java
│   │   │   └── resources/
│   │   │       └── application.properties
│   ├── target/
│   │   └── motion-api-1.0.0.jar    # Built API
│   ├── libs/
│   │   └── libmotionwrapper.so     # JNI library to be loaded at runtime
│   ├── Dockerfile                  # Builds Docker image
│   ├── spring-motion-deployment.yaml  # Kubernetes deployment manifest
│   ├── spring-motion-service.yaml     # Kubernetes service manifest
│   └── pom.xml                     # Maven project descriptor

├── scripts/                        # Optional helper scripts
│   ├── build.sh
│   └── run-local.sh

└── README.md                       # Documentation

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
