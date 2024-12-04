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

    Sensor sensor(BP);
    while(isMonitoring && !stopFlag.load()) {

        int leftValue = sensor.returnUltrasonicValue(4);
        int rightValue = sensor.returnUltrasonicValue(3);

        updateBuffers(leftValue, rightValue);

        IntersectionCheckerResult result = {false, false, false, false};
        result.left = (leftValue > 20);
        result.right = (rightValue > 20);
        result.specialCase1 = checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersection(leftValue, rightValue);
        result.specialCase2 = checkCaseWhereRobotIsTooCloseToWallWithTheIntersection(leftValue, rightValue);

        if(result.left || result.right) {
            checkerFlag.store(true);
        }

        {
            lock_guard<mutex> lock(resultMutex);
            latestResult = result;
        }

        constexpr float second = 1000000.0;
        usleep(second/8);
    }
}

void CheckForIntersection::updateBuffers(int leftValue, int rightValue) {
    if(leftSensorBuffer.size() >= BUFFER_SIZE) {
        leftSensorBuffer.pop_front();
    }
    if(rightSensorBuffer.size() >= BUFFER_SIZE) {
        rightSensorBuffer.pop_front();
    }
    leftSensorBuffer.push_back(leftValue);
    rightSensorBuffer.push_back(rightValue);
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersection(int leftValue, int rightValue) {
    if(leftValue > 20) {
        if(rightSensorBuffer[9] < 7 && rightSensorBuffer[8] < 7) {
            return true;
        }
    }
    if(rightValue > 20) {
        if(leftSensorBuffer[9] < 7 && leftSensorBuffer[8] < 7) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersection(int leftValue, int rightValue) {
    if(leftValue > 20) {
        if(leftSensorBuffer[9] < 7 && leftSensorBuffer[8] < 7){
            return true;
        }
    }
    if(rightValue > 20) {
        if(rightSensorBuffer[9] < 7 && rightSensorBuffer[8] < 7) {
            return true;
        }
    }
    return false;
}


IntersectionCheckerResult CheckForIntersection::getLatestResult() {
    lock_guard<mutex> lock(resultMutex);
    return latestResult;
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

