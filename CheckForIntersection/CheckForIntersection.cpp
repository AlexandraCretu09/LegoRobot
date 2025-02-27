//
// Created by alexa on 26/11/24.
//
#include <iostream>
#include "CheckForIntersection.h"

#include <atomic>
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

        float leftValue = sensor.returnUltrasonicValue(4);
        float rightValue = sensor.returnUltrasonicValue(3);
        float forwardValue = sensor.returnUltrasonicValue(2);


        if(leftValue < 15 && rightValue < 15) {
            updateBuffers(leftValue, rightValue);
        }

        IntersectionCheckerResult result = {false, false, false, false, false};
        result.left = (leftValue > 20);
        result.right = (rightValue > 20);
        result.forward = (forwardValue > 20);
        result.specialCase1 = checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersection(leftValue, rightValue);
        result.specialCase2 = checkCaseWhereRobotIsTooCloseToWallWithTheIntersection(leftValue, rightValue);


        if(result.left || result.right) {
            checkerFlag.store(true);
            stopFlag.store(true);
        }

        if (!result.left && !result.right && !result.forward) {
            if (forwardValue <= 7) {
                printf("Forward value: %f\n", forwardValue);
                result.deadend = true;
                checkerFlag.store(true);
                stopFlag.store(true);
            }
        }

        {
            lock_guard<mutex> lock(resultMutex);
            latestResult = result;
        }

        printf("Right value: %f\n", rightValue);
        printf("Left value: %f\n", leftValue);
        printf("Forward value: %f\n", forwardValue);

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
        if(rightSensorBuffer[4] < 7 && rightSensorBuffer[3] < 7) {
            return true;
        }
    }
    if(rightValue > 20) {
        if(leftSensorBuffer[4] < 7 && leftSensorBuffer[3] < 7) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersection(int leftValue, int rightValue) {
    if(leftValue > 20) {
        if(leftSensorBuffer[4] <= 7 && leftSensorBuffer[3] <= 7){
            return true;
        }
    }
    if(rightValue > 20) {
        if(rightSensorBuffer[4] <= 7 && rightSensorBuffer[3] <= 7) {
            return true;
        }
    }
    return false;
}


IntersectionCheckerResult CheckForIntersection::getLatestResult() {
    lock_guard<mutex> lock(resultMutex);
    printf("Right: %d\n Left: %d\nForward: %d\n\n", latestResult.left, latestResult.right, latestResult.forward);
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

