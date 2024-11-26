//
// Created by alexa on 22/10/24.
//
#include "GyroMonitor.h"

#include <iostream>
#include <ctime>

#include "GyroMonitor.h"
#include "../Sensors/getSensorData.h"
#include "../common.h"

#include <brickpi_types.h>
#include <unistd.h>

using namespace std;
float second2 = 1000000.0;
GyroMonitor::GyroMonitor(atomic<bool>& stopFlag, BrickPi3& BP)
    : isMonitoring(false), stopFlag(stopFlag), BP(BP) {
}

int GyroMonitor::getGyroscopeReading() {
    Sensor sensor(BP);
    sensor_gyro_t gyroValues = sensor.returnGyroValue();
    return gyroValues.abs;
}

bool GyroMonitor::isRobotStuck() {
    if(gyroBuffer.size() < maxSize ) {
        return false;
    }

    int firstValue = gyroBuffer.front();

    printf("Gyro buffer: ");
    for(int value : gyroBuffer) {
        printf("%d ", value);
    }
    printf("\n");

    for(int value : gyroBuffer) {

        if(value < firstValue - 5 || value > firstValue + 5) {
            printf("Robot not stuck");
            return false;
        }
    }

    return true;
}

void GyroMonitor::monitorGyroscope() {
    printf("Started monitor thread\n");
    while(isMonitoring) {
        int newReading = getGyroscopeReading();
        printf("New reading: %d", newReading);

        gyroBuffer.push_back(newReading);
        if(gyroBuffer.size() > maxSize ) {
            gyroBuffer.pop_front();
        }

        if(isRobotStuck()) {
            stopFlag.store(true);
            printf("Robot might be stuck!\n");
        }

        usleep(second2/2);
    }
}

void GyroMonitor::startMonitoring() {
    isMonitoring = true;
    printf("In monitoring\n");
    monitorThread = thread(&GyroMonitor::monitorGyroscope, this);
}

void GyroMonitor::stopMonitoring() {
    if(isMonitoring) {
        isMonitoring = false;
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
    }
}


