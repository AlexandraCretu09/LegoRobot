//
// Created by alexa on 26/11/24.
//
#include <iostream>
#include "CheckForIntersection.h"

#include <atomic>
#include <set>
#include <unistd.h>

#include "../Movement/wheelsMovement.h"
#include "../Motor/motor.h"
#include "../Sensors/getSensorData.h"
#include "../common.h"


using namespace std;

CheckForIntersection::CheckForIntersection(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, atomic<bool> &waiterForIntersectionResult, BrickPi3 &BP)
    : isMonitoring(false), stopFlag(stopFlag), checkerFlag(checkerFlag), waiterForIntersectionResult(waiterForIntersectionResult), BP(BP) {

}

void CheckForIntersection::checkUntilRobotPassedIntersection() {
    Sensor sensor(BP);
    WheelsMovement move(BP);
    Motor motor(BP);

    motor.resetBothMotorEncoders();

    float leftValue, rightValue;

    printf("in case where robot has to pass intersection\n");

    int countLeft = 0, countRight = 0;
    bool ok = true;
    clock_t start = clock();

    while ((countRight < 3 || countLeft < 3) && !stopFlag.load()) {
        if (ok) {
            move.goForward();
            ok = false;
        }

        leftValue = sensor.returnUltrasonicValue(4);
        rightValue = sensor.returnUltrasonicValue(3);
        // printf("left: %.2f, right: %.2f\n", leftValue, rightValue);

        if (rightValue <= 20)
            countRight++;
        if (leftValue <= 20)
            countLeft++;

        // retryCount++;
        // if (retryCount > 5000) {
        //     printf("Timeout: sensor values didn't drop for passing interseciton\n");
        //     move.stop();
        //     move.goBackwards(-150);
        //     break;
        // }

        clock_t now = clock();
        double elapsed = (double)(now - start) / CLOCKS_PER_SEC;

        if (elapsed > 2.0) {
            printf("Timeout: sensor values didn't drop for passing intersection\n");
            move.stop();
            move.goBackwards(-150);
            break;
        }
    }
    move.goForward(float(0.25));

    move.stop();
    printf("exited case where robot has to pass intersection\n");

}




void CheckForIntersection::checkUntilRobotReachIntersectionAgain() {
    Sensor sensor(BP);
    WheelsMovement move(BP);
    Motor motor(BP);

    motor.resetBothMotorEncoders();

    float leftValue, rightValue;


    printf("\nin case where robot has to reach intersection\n");
    int countLeft = 0, countRight = 0;
    bool ok = true;
    clock_t start = clock();


    while (countRight < 2 && countLeft < 2 && !stopFlag.load()) {
        if (ok) {
            move.goForward();
            ok = false;
        }

        leftValue = sensor.returnUltrasonicValue(4);
        rightValue = sensor.returnUltrasonicValue(3);
        // printf("left: %.2f, right: %.2f\n", leftValue, rightValue);

        if (rightValue > 20)
            countRight++;
        if (leftValue > 20)
            countLeft++;

        // retryCount++;
        // if (retryCount > 4500) {
        //     printf("Timeout: sensor values didn't drop for reaching intersection\n");
        //     move.stop();
        //     move.goBackwards(-150);
        //     break;
        // }

        clock_t now = clock();
        double elapsed = (double)(now - start) / CLOCKS_PER_SEC;

        if (elapsed > 2.0) {
            printf("Timeout: sensor values didn't drop for reaching intersection\n");
            move.stop();
            move.goBackwards(-150);
            break;
        }
    }
    move.goForward(float(0.25));
    move.stop();
    printf("exited case where robot has to reach intersection\n");

}

deadendSpecialCases CheckForIntersection::checkIfDeadendPositionIsInASpecialCase() {
    Sensor sensor(BP);
    deadendSpecialCases result = {false, false};
    int countLeft = 0, countRight = 0, overallCounter=0;
    while (overallCounter < 3 && !stopFlag.load()) {
        if (sensor.returnUltrasonicValue(4) <= 7) {
            countLeft++;
        }
        if (sensor.returnUltrasonicValue(3) <= 7) {
            countRight++;
        }
        overallCounter++;
    }
    if (countLeft) {
        result.tooCloseToTheLeftWall = true;
    }
    if (countRight) {
        result.tooCloseToTheRightWall = true;
    }
    return result;

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




        move.goForward(float(0.25));


        if (result.left || result.right || result.deadend) {
            if (!result.left)
                result.left = checkSensorValue(4, sensor);
            if (!result.right)
                result.right = checkSensorValue(3, sensor);
            if (!result.forward)
            result.forward = checkSensorValue(2, sensor);

            if (result.deadend) {
                result.forward = result.right = result.left = 0;
            }

            //printf("Finished storing information about intersection\n");

            waiterForIntersectionResult.store(true);

            {
                // printf("Finished storing result\n");
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
        if(leftSensorBuffer[1] <= 8 && leftSensorBuffer[2] <= 8) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionRight(int leftValue) {
    if(leftValue > 20) {
        if(rightSensorBuffer[1] <= 8 && rightSensorBuffer[2] <= 8) {
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionLeft(int leftValue, int rightValue) {
    if(leftValue > 20) {
        if(leftSensorBuffer[1] <= 8 && leftSensorBuffer[2] <= 8){
            return true;
        }
    }
    return false;
}

bool CheckForIntersection::checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionRight(int leftValue, int rightValue) {
    if(rightValue > 20) {
        if(rightSensorBuffer[1] <= 8 && rightSensorBuffer[2] <= 8) {
            return true;
        }
    }
    return false;
}


IntersectionCheckerResult CheckForIntersection::getLatestResult() {
    lock_guard<mutex> lock(resultMutex);
    // printf("left sensor buffer: 1: %d, 2: %d\n", leftSensorBuffer[1], leftSensorBuffer[2]);
    // printf("Right sensor buffer: 1: %d, 2: %d\n", rightSensorBuffer[1], rightSensorBuffer[2]);
    // printf("SP1L: %d\nSP1R: %d\nSP2L: %d\nSP2R:%d\n\n", latestResult.specialCase1Left, latestResult.specialCase1Right, latestResult.specialCase2Left, latestResult.specialCase2Right);
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
        if (sensorValueBuffer [i] > 20) {
            return true;
        }
    }
    return false;

}
