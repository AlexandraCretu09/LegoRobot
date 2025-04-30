//
// Created by alexa on 22/10/24.
//
#include "MonitorIfStuck.h"

#include <iostream>
#include <ctime>

#include "MonitorIfStuck.h"

#include <algorithm>
#include <atomic>

#include "../Sensors/getSensorData.h"
#include "../common.h"

#include <brickpi_types.h>
#include <set>
#include <unistd.h>

using namespace std;
float second2 = 1000000.0;
MonitorIfStuck::MonitorIfStuck(atomic<bool>& stopFlag, atomic<bool> &checkerForFrontBlock, BrickPi3& BP)
    : isMonitoring(false), stopFlag(stopFlag), checkerForFrontBlock(checkerForFrontBlock), BP(BP) {
}

int MonitorIfStuck::getGyroscopeReading() {
    Sensor sensor(BP);
    int gyroValues = sensor.returnGyroValue();
    return gyroValues;
}

int MonitorIfStuck::getFrontSensorReading() {
    Sensor sensor(BP);
    int frontSensorValue = sensor.returnUltrasonicValue(2);
    return frontSensorValue;
}

void MonitorIfStuck::monitorGyroAndFrontSensor() {
    printf("\nStarted monitor thread\n");
    int count = 0;
    while(isMonitoring && !stopFlag.load()) {
        int gyroValue = getGyroscopeReading();
        int frontValue = getFrontSensorReading();

        updateFrontBuffer(gyroValue);
        updateGyroBuffer(frontValue);
        count++;

        if (count >= BUFFER_SIZE) {
            // printf("Counter bigger than buffer size\n");
            if(/*isRobotStuckByGyro() ||*/ isRobotIsStuckByFrontWall()) {
                stopFlag.store(true);
                checkerForFrontBlock.store(true);
            }
        }

        usleep(second2/2);
    }
}


bool MonitorIfStuck::isRobotStuckByGyro() {
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


bool MonitorIfStuck::isRobotIsStuckByFrontWall() {
    const int LOW_STUCK_ZONE = -999;
    const int HIGH_STUCK_ZONE = 35;

    const int STUCK_ZONE_MIN_COUNT = 8;

    const int REQUIRED_DECREASING_COUNT = 2;
    const int MIN_VALID_DISTANCE = 0;
    const int MAX_VALID_DISTANCE = 200;

    const int MIN_DECREASE_DELTA = 3;

    int stuckZoneCount = 0;
    int decreasingCount = 0;

    for (int i = 1; i < BUFFER_SIZE; i++) {
        int current = frontSensorBuffer[i];
        int prev = frontSensorBuffer[i - 1];

        if (current < MIN_VALID_DISTANCE || current > MAX_VALID_DISTANCE) {
            stuckZoneCount++;
            continue;
        }
        if (prev < MIN_VALID_DISTANCE || prev > MAX_VALID_DISTANCE) {
            prev = current;
        }
        if (current >= LOW_STUCK_ZONE && current <= HIGH_STUCK_ZONE) {
            stuckZoneCount++;
        }

        if (prev - current >= MIN_DECREASE_DELTA) {
            decreasingCount++;
        } else {
            decreasingCount = 0;
        }

    }

    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     printf("Front sensor values: %d\n", frontSensorBuffer[i]);
    // }

    if (stuckZoneCount >= STUCK_ZONE_MIN_COUNT) {
        if (decreasingCount < REQUIRED_DECREASING_COUNT) {
            printf("Robot is stuck: repeated low values and no clear movement.\n");
            return true;
        }
    }
    return false;
}


void MonitorIfStuck::updateGyroBuffer(int gyroValue) {
    if(frontSensorBuffer.size() >= BUFFER_SIZE) {
        gyroBuffer.pop_front();
    }
    frontSensorBuffer.push_back(gyroValue);
}

void MonitorIfStuck::updateFrontBuffer(int frontValue) {
    if(frontSensorBuffer.size() >= BUFFER_SIZE) {
        frontSensorBuffer.pop_front();
    }
    frontSensorBuffer.push_back(frontValue);
}


void MonitorIfStuck::startMonitoring() {
    isMonitoring = true;
    printf("In monitoring\n");
    monitorThread = thread(&MonitorIfStuck::monitorGyroAndFrontSensor, this);
}

void MonitorIfStuck::stopMonitoring() {
    if(isMonitoring) {
        isMonitoring = false;
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
    }
}


