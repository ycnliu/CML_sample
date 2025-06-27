#include "com_cml_wrapper_MotionSystemWrapper.h"
#include "MotionController.h"
#include "../src/CalibrationManager.h"
#include "../src/TriggerHandler.h"
#include "../src/SafetyMonitor.h"
#include "../src/Logger.h"

// Create global components
static CalibrationManager calib;
static TriggerHandler trigger;
static SafetyMonitor safety;
static Logger logger;
static MotionController controller(calib, trigger, safety, logger, 2); // Assuming 2 axes for this example

JNIEXPORT void JNICALL Java_com_cml_wrapper_MotionSystemWrapper_initialize(JNIEnv*, jobject) {
    controller.initialize();
}

JNIEXPORT void JNICALL Java_com_cml_wrapper_MotionSystemWrapper_homeAll(JNIEnv*, jobject) {
    controller.homeAll();
}

JNIEXPORT void JNICALL Java_com_cml_wrapper_MotionSystemWrapper_moveTo(JNIEnv* env, jobject, jdoubleArray positions) {
    jsize len = env->GetArrayLength(positions);
    jdouble* pos = env->GetDoubleArrayElements(positions, 0);
    std::vector<double> cpp_positions(pos, pos + len);
    env->ReleaseDoubleArrayElements(positions, pos, 0);
    controller.moveTo(cpp_positions);
}
