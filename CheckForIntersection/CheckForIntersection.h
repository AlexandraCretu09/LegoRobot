//
// Created by alexa on 26/11/24.
//

#ifndef CHECKFORINTERSECTION_H
#define CHECKFORINTERSECTION_H
#include <atomic>
#include <BrickPi3.h>
#include <deque>
#include <thread>

#include "../common.h"
class Sensor;
using namespace std;



class CheckForIntersection {
public:
    mutex resultMutex;

    CheckForIntersection(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, atomic<bool> &waiterForIntersectionResult, BrickPi3 &BP);
    IntersectionCheckerResult getLatestResult();
    void startMonitoring();
    void stopMonitoring();
    void checkUntilRobotPassedIntersection(int *leftValues, int *rightValues);
    void checkUntilRobotReachIntersectionAgain();
    deadendAndIntersectionSpecialCases checkIfDeadendPositionIsInASpecialCase();
    deadendAndIntersectionSpecialCases checkIfIntersectionPositionIsInASpecialCase(int leftValues[], int rightValues[]);


private:
    atomic <bool> &stopFlag;
    atomic<bool> &checkerFlag;
    atomic<bool> &waiterForIntersectionResult;
    BrickPi3& BP;
    deque<int> leftSensorBuffer;
    deque<int> rightSensorBuffer;
    IntersectionCheckerResult latestResult = {false, false, false, false, false, false, false};
    static constexpr size_t BUFFER_SIZE = 3;

    bool isMonitoring;
    thread checkForIntersectionThread;


    void checker();

    void updateLeftBuffer(int leftValue);
    void updateRightBuffer(int rightValue);

    bool checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionLeft(int rightValue);
    bool checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionRight(int leftValue);
    bool checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionLeft(int leftValue, int rightValue);
    bool checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionRight(int leftValue, int rightValue);

    bool checkSensorValue(int sensorNumber, Sensor sensor);
};
#endif //CHECKFORINTERSECTION_H
