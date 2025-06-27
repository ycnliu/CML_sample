package com.example.motion;

import com.cml.wrapper.MotionSystemWrapper;
import org.springframework.stereotype.Service;

@Service
public class MotionControllerService {
    private static final MotionSystemWrapper wrapper = new MotionSystemWrapper();

    public void initialize() {
        wrapper.initialize();
    }

    public void homeAll() {
        wrapper.homeAll();
    }

    public void moveTo(double[] positions) {
        wrapper.moveTo(positions);
    }

}

