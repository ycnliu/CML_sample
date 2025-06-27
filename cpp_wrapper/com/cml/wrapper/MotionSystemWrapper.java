package com.cml.wrapper;

public class MotionSystemWrapper {
    static {
        System.loadLibrary("motionwrapper");
    }

    public native void initialize();
    public native void homeAll();
    public native void moveTo(double[] positions);
}