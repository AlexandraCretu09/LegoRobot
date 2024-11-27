//
// Created by alexa on 26/11/24.
//

#ifndef CHECKFORINTERSECTION_H
#define CHECKFORINTERSECTION_H
#include <BrickPi3.h>
#include <thread>

#include "../common.h"
using namespace std;

class CheckForIntersection {
public:
    CheckForIntersection(atomic<bool> &stopFlag,atomic<bool> &checkerFlag, BrickPi3 &BP);
    void startMonitoring();
    void stopMonitoring();
private:
    bool isMonitoring;
    thread checkForIntersectionThread;
    atomic <bool> &stopFlag;
    atomic<bool> &checkerFlag;
    BrickPi3& BP;

    void checker();
};
#endif //CHECKFORINTERSECTION_H
