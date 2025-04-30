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

class MonitorIfStuck {
public:
    MonitorIfStuck(atomic<bool>& stopFlag, atomic<bool> &checkerForFrontBlock, BrickPi3& BP);
    void startMonitoring();
    void stopMonitoring();

private:
    deque<int> gyroBuffer;
    deque<int> frontSensorBuffer;
    const int maxSize = 10;
    static constexpr size_t BUFFER_SIZE = 10;
    atomic<bool> &checkerForFrontBlock;
    atomic<bool> &stopFlag;

    bool isMonitoring;
    thread monitorThread;
    BrickPi3& BP;

    void monitorGyroAndFrontSensor();
    bool isRobotStuckByGyro();
    int getGyroscopeReading();
    int getFrontSensorReading();

    void updateFrontBuffer(int frontValue);
    //void initializeFrontBuffer();
    void updateGyroBuffer(int gyroValue);
    bool isRobotIsStuckByFrontWall();

};

#endif //MONITORGYROSCOPE_H
