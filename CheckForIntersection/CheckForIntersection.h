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
using namespace std;



class CheckForIntersection {
public:
    CheckForIntersection(atomic<bool> &stopFlag,atomic<bool> &checkerFlag, BrickPi3 &BP);
    void startMonitoring();
    void stopMonitoring();
    mutex resultMutex;
    IntersectionCheckerResult getLatestResult();

private:
    atomic <bool> &stopFlag;
    atomic<bool> &checkerFlag;
    BrickPi3& BP;
    deque<int> leftSensorBuffer;
    deque<int> rightSensorBuffer;
    IntersectionCheckerResult latestResult = {false, false, false, false, false};
    static constexpr size_t BUFFER_SIZE = 3;

    bool isMonitoring;
    thread checkForIntersectionThread;
    void checker();
    void updateBuffers(int leftValue, int rightValue);
    void updateLeftBuffer(int leftValue);
    void updateRightBuffer(int rightValue);
    bool checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionLeft(int rightValue);
    bool checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersectionRight(int leftValue);
    bool checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionLeft(int leftValue, int rightValue);
    bool checkCaseWhereRobotIsTooCloseToWallWithTheIntersectionRight(int leftValue, int rightValue);

};
#endif //CHECKFORINTERSECTION_H
