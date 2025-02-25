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
    static constexpr size_t BUFFER_SIZE = 5;

    bool isMonitoring;
    thread checkForIntersectionThread;
    void checker();
    void updateBuffers(int leftValue, int rightValue);
    bool checkCaseWhereRobotIsTooCloseToOppositeWallOfIntersection(int leftValue, int rightValue);
    bool checkCaseWhereRobotIsTooCloseToWallWithTheIntersection(int leftValue, int rightValue);

};
#endif //CHECKFORINTERSECTION_H
