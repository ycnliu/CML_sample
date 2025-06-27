CML JNI Wrapper
===============

This module bridges C++ motion control logic (using a simulated Copley Motion Library)
with Java via JNI. It enables Java applications to initialize and control a multi-axis
motion system through native C++ code.

Project Structure:
------------------
cpp_wrapper/
├── com/
│   └── cml/
│       └── wrapper/
│           └── MotionSystemWrapper.java     # Java JNI wrapper class
├── MotionSystemWrapper.cpp                  # C++ native JNI implementation
├── com_cml_wrapper_MotionSystemWrapper.h    # Auto-generated JNI header
├── TestJNI.java                             # Java test harness
├── libmotionwrapper.dylib                   # Compiled native dynamic library (macOS)

Prerequisites:
--------------
- JDK 17+
- g++ or compatible C++17 compiler
- macOS or Linux (edit Makefile as needed for your OS)

Build Instructions:
-------------------
1. Generate JNI header from Java class:
   $ javac -h . com/cml/wrapper/MotionSystemWrapper.java

2. Compile the native C++ dynamic library:
   $ make

   This builds: libmotionwrapper.dylib

3. Compile and run Java test:
   $ javac -d . TestJNI.java
   $ java -Djava.library.path=. TestJNI

Expected Output:
----------------
Primary axis initialized
Axis 2 initialized
MotionController initialization complete
JNI call completed.

Troubleshooting:
----------------
- UnsatisfiedLinkError:
  -> Ensure libmotionwrapper.dylib is in the current directory
  -> Ensure the name matches: System.loadLibrary("motionwrapper")

- jni.h not found:
  -> Make sure your JDK headers are included in the Makefile:
     e.g. /Library/Java/JavaVirtualMachines/openjdk.jdk/Contents/Home/include

- Architecture mismatch:
  -> Confirm Java and C++ targets (x86_64 or arm64) match your platform

Notes:
------
- This JNI interface links to MotionController and its required subsystems
- Make sure Logger, SafetyMonitor, TriggerHandler, CalibrationManager are all linked

License:
--------
MIT License (JNI wrapper only; CML usage may require proprietary licensing)

