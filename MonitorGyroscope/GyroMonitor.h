//
// Created by alexa on 22/10/24.
//

#ifndef MONITORGYROSCOPE_H
#define MONITORGYROSCOPE_H

#include <atomic>
#include <BrickPi3.h>
#include <deque>
#include <thread>
using namespace std;

class GyroMonitor {
public:
    GyroMonitor(atomic<bool>& stopFlag, BrickPi3& BP);
    void startMonitoring();
    void stopMonitoring();
private:
    deque<int> gyroBuffer;
    bool isMonitoring;
    thread monitorThread;
    const int maxSize = 10;

    atomic<bool>& stopFlag;
    BrickPi3& BP;
    void monitorGyroscope();
    bool isRobotStuck();
    int getGyroscopeReading();

};

#endif //MONITORGYROSCOPE_H
