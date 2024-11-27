//
// Created by alexa on 26/11/24.
//
#include <iostream>
#include "CheckForIntersection.h"

#include <unistd.h>

#include "../Movement/wheelsMovement.h"
#include "../Sensors/getSensorData.h"
#include "../common.h"


using namespace std;

CheckForIntersection::CheckForIntersection(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, BrickPi3 &BP)
    : isMonitoring(false), stopFlag(stopFlag), checkerFlag(checkerFlag),  BP(BP) {
}


void CheckForIntersection::checker() {

    while(isMonitoring && !stopFlag.load()) {
        Sensor sensor(BP);
        IntersectionCheckerResult result = {false, false};

        if(sensor.returnUltrasonicValue(4) > 20) {
            result.left = true;
        }
        if(sensor.returnUltrasonicValue(3) > 20)
            result.right = true;

        if(result.left || result.right) {
            checkerFlag.store(true);
        }
        constexpr float second = 1000000.0;
        usleep(second/2);
    }
}

void CheckForIntersection::startMonitoring() {
    isMonitoring = true;
    checkForIntersectionThread = thread(&CheckForIntersection::checker, this);
}

void CheckForIntersection::stopMonitoring() {
    if(isMonitoring) {
        isMonitoring = false;
        if(checkForIntersectionThread.joinable())
            checkForIntersectionThread.join();
    }
}

