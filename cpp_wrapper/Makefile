# Makefile for building JNI dynamic library

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall

# Java JNI include paths (macOS specific)
JAVA_HOME = /Library/Java/JavaVirtualMachines/openjdk.jdk/Contents/Home
JNI_INCLUDE = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin

# Include and source paths
INCLUDE = -I../include -I../src
SRC = \
  MotionSystemWrapper.cpp \
  ../src/MotionController.cpp \
  ../src/CalibrationManager.cpp \
  ../src/TriggerHandler.cpp \
  ../src/SafetyMonitor.cpp \
  ../src/Logger.cpp

# Output dynamic library
OUT = libMotionSystemWrapper.dylib

# Build rule
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(JNI_INCLUDE) $(INCLUDE) $^ -dynamiclib -o $@

clean:
	rm -f $(OUT) *.o

