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

    rightSensorBuffer[1] = 99;
    rightSensorBuffer[2] = 99;
    leftSensorBuffer[1] = 99;
    leftSensorBuffer[2] = 99;

    Sensor sensor(BP);
    while(isMonitoring && !stopFlag.load()) {

        float leftValue = sensor.returnUltrasonicValue(4);
        float rightValue = sensor.returnUltrasonicValue(3);
        float forwardValue = sensor.returnUltrasonicValue(2);



        if (leftValue < 15) {
            updateLeftBuffer(leftValue);
        }
        if (rightValue < 15) {
            updateRightBuffer(rightValue);
        }

        IntersectionCheckerResult result = {false, false, false, false, false};
        result.left = (leftValue > 20);
        result.right = (rightValue > 20);
        result.forward = (forwardValue > 20);
        result.specialCase1Left = checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionLeft(rightValue);
        result.specialCase1Right = checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionRight(leftValue);
        result.specialCase2Left = checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionLeft(leftValue, rightValue);
        result.specialCase2Right = checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionRight(leftValue, rightValue);


        if(result.left || result.right) {
            checkerFlag.store(true);
            stopFlag.store(true);
        }

        if (!result.left && !result.right && !result.forward) {
            if (forwardValue <= 7) {
                // printf("Forward value: %f\n", forwardValue);
                result.deadend = true;
                checkerFlag.store(true);
                stopFlag.store(true);
            }
        }

        {
            lock_guard<mutex> lock(resultMutex);
            latestResult = result;
        }

        // printf("Right value: %f\n", rightValue);
        // printf("Left value: %f\n", leftValue);
        // printf("Forward value: %f\n", forwardValue);

        constexpr float second = 1000000.0;
        usleep(second/8);
    }
}


void CheckForIntersection::updateLeftBuffer(int leftValue) {
    if(leftSensorBuffer.size() >= BUFFER_SIZE) {
        leftSensorBuffer.pop_front();
    }
    leftSensorBuffer.push_back(leftValue);
}

void CheckForIntersection::updateRightBuffer(int rightValue) {
    if(rightSensorBuffer.size() >= BUFFER_SIZE) {
        rightSensorBuffer.pop_front();
    }
    rightSensorBuffer.push_back(rightValue);
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionLeft(int rightValue) {

    if(rightValue > 20) {
        if(leftSensorBuffer[1] <= 7 && leftSensorBuffer[2] <= 7) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionRight(int leftValue) {
    if(leftValue > 20) {
        if(rightSensorBuffer[1] <= 7 && rightSensorBuffer[2] <= 7) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionLeft(int leftValue, int rightValue) {
    if(leftValue > 20) {
        if(leftSensorBuffer[1] <= 7 && leftSensorBuffer[2] <= 7){
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionRight(int leftValue, int rightValue) {
    if(rightValue > 20) {
        if(rightSensorBuffer[1] <= 7 && rightSensorBuffer[2] <= 7) {
            return true;
        }
    }
    return false;
}


IntersectionCheckerResult CheckForIntersection::getLatestResult() {
    lock_guard<mutex> lock(resultMutex);
    // printf("left sensor buffer: 1: %d, 2: %d\n", leftSensorBuffer[1], leftSensorBuffer[2]);
    // printf("Right sensor buffer: 1: %d, 2: %d\n", rightSensorBuffer[1], rightSensorBuffer[2]);
    // printf("Left: %d\n Right: %d\nForward: %d\n\n", latestResult.left, latestResult.right, latestResult.forward);
    return latestResult;
}


void CheckForIntersection::startMonitoring() {
    isMonitoring = true;
    // printf("Monitoring thread..");
    checkForIntersectionThread = thread(&CheckForIntersection::checker, this);
}

void CheckForIntersection::stopMonitoring() {
    // printf("Not monitoring thread..");
    if(isMonitoring) {
        isMonitoring = false;
        if(checkForIntersectionThread.joinable())
            checkForIntersectionThread.join();
    }
}
//ToDo Better storing of intersection values