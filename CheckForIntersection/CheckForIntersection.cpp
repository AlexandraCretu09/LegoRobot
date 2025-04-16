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

CheckForIntersection::CheckForIntersection(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, atomic<bool> &waiterForIntersectionResult, BrickPi3 &BP)
    : isMonitoring(false), stopFlag(stopFlag), checkerFlag(checkerFlag), waiterForIntersectionResult(waiterForIntersectionResult) ,BP(BP) {

}

void CheckForIntersection::checkUntilRobotPassedIntersection() {
    Sensor sensor(BP);
    WheelsMovement move(BP);
    float leftValue;
    float rightValue;
    printf("in case where both right and left\n");
    leftValue = sensor.returnUltrasonicValue(4);
    rightValue = sensor.returnUltrasonicValue(3);

    int retryCount = 0;
    bool ok = true;
    while ((rightValue > 20 || leftValue > 20) && !stopFlag.load()) {
        if (ok) {
            move.goForward();
            ok = false;
        }

        leftValue = sensor.returnUltrasonicValue(4);
        rightValue = sensor.returnUltrasonicValue(3);
        // printf("left: %.2f, right: %.2f\n", leftValue, rightValue);

        retryCount++;
        if (retryCount > 30000) {
            printf("Timeout: sensor values didn't drop\n");
            break;
        }
        // usleep(50 * 1000);
    }
    move.stop();
    printf("exited case where both right and left\n");

}



void CheckForIntersection::checker() {

    rightSensorBuffer[1] = 99;
    rightSensorBuffer[2] = 99;
    leftSensorBuffer[1] = 99;
    leftSensorBuffer[2] = 99;

    WheelsMovement move(BP);
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
        result.right = (rightValue > 20);
        // if (rightValue>20){printf("right value: %f\n", rightValue);}
        result.left = (leftValue > 20);
        // if (leftValue > 20) {printf("left value: %f\n", leftValue);}
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




        move.goForward(0.25);

        // printf("Finished storing information about intersection\n");
        if (result.left || result.right || result.deadend) {
            if (!result.left)
                result.left = checkSensorValue(4, sensor);
            if (!result.right)
                result.right = checkSensorValue(3, sensor);
            if (!result.forward)
            result.forward = checkSensorValue(2, sensor);

            waiterForIntersectionResult.store(true);

            {
                lock_guard<mutex> lock(resultMutex);
                latestResult = result;
            }
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

bool CheckForIntersection::checkSensorValue(int sensorNumber, Sensor sensor) {
    int sensorValueBuffer[10] = {0};
    for (int i=0;i<5;i++) {
        sensorValueBuffer[i] = sensor.returnUltrasonicValue(sensorNumber);
        usleep(1000000/32);
    }
    for (int i=0;i<5;i++) {
        // if (sensorNumber == 4)
        //     // printf("Values for left sensor: %d\n", sensorValueBuffer[i]);
        // if (sensorNumber == 3)
            // printf("Values for right sensor: %d\n", sensorValueBuffer[i]);
        if (sensorValueBuffer [i] > 20) {
            return true;
        }
    }
    return false;

}
