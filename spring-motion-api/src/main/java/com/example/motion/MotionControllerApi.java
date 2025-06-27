package com.example.motion;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/motion")
public class MotionControllerApi {


    @Autowired
    private MotionControllerService service;

    @PostMapping("/initialize")
    public String initialize() {
        service.initialize();
        return "Initialized.";
    }

    @PostMapping("/home")
    public String homeAll() {
        service.homeAll();
        return "Homing complete.";
    }

    @PostMapping("/move")
    public String moveTo(@RequestBody double[] positions) {
        service.moveTo(positions);
        return "Move command sent.";
    }

}

