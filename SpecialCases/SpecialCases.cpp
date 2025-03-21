//
// Created by alexa on 04/12/24.
//

#include "SpecialCases.h"
#include "BrickPi3.h"
#include "../Sensors/getSensorData.h"
#include "../Movement/wheelsMovement.h"
#include "../Movement/rotation.h"
#include "../Motor/motor.h"
#include <atomic>
#include <cmath>
#include<unistd.h>


using namespace std;
// float second3 = ;
SpecialCases::SpecialCases(BrickPi3 BP) : BP(BP) {}

void SpecialCases::toCloseToTheRight(Sensor &sensorObj, BrickPi3 &BP) {  // Pass Sensor object as parameter
    float rightSensor = sensorObj.returnUltrasonicValue(4); // Call function through Sensor object

    if (rightSensor <= 6) {  // Too close to the right wall
        Motor motor(BP);  // Cre1000000.0ate Motor object

        // Move backward while turning left (right wheel faster)
        motor.setRightWheelDPS(-250);  // Right wheel moves faster
        motor.setLeftWheelDPS(-150);   // Left wheel moves slower
        usleep(1000000.0*1.2);  // Move for 1 second

        // Rectify position (left wheel moves faster)
        motor.setRightWheelDPS(-100);  // Right wheel slower
        motor.setLeftWheelDPS(-310);   // Left wheel faster
        usleep(1000000.0/2);

        // Stop the motors
        motor.setRightWheelDPS(0);
        motor.setLeftWheelDPS(0);
    }
}
void SpecialCases::toCloseToTheLeft(Sensor &sensorObj, BrickPi3 &BP) {
    float leftSensor = sensorObj.returnUltrasonicValue(3); // Get left sensor data

    if (leftSensor <= 6) {  // Too close to the left wall
        Motor motor(BP);  // Create Motor object

        // Move backward while turning right (left wheel faster)
        motor.setLeftWheelDPS(-250);   // Left wheel moves faster
        motor.setRightWheelDPS(-150);  // Right wheel moves slower
        usleep(1000000.0*1.2);  // Move for 1 second

        // Rectify position (right wheel moves faster)
        motor.setLeftWheelDPS(-100);   // Left wheel slower
        motor.setRightWheelDPS(-310);  // Right wheel faster
        usleep(1000000.0/2);  // Move for 0.5 seconds

        // Stop the motors
        motor.setLeftWheelDPS(0);
        motor.setRightWheelDPS(0);
    }
}

void SpecialCases::frontBlocked(Sensor &sensorObj, BrickPi3 &BP) {
    float frontSensor = sensorObj.returnUltrasonicValue(2); // Get front sensor data

    if (frontSensor <= 6) {  // Dead end detected
        Rotation rotation(BP);  // Create Rotation object
        std::atomic<bool> stopFlag(false);

        rotation.rotateBackwards(stopFlag); // Rotate 180° backward
    }
}
void SpecialCases::cornerTrapRight(Sensor &sensorObj, BrickPi3 &BP) {
    float rightSensor = sensorObj.returnUltrasonicValue(4); // Get right sensor data
    float frontSensor = sensorObj.returnUltrasonicValue(1); // Get front sensor data
    Motor motor(BP);
    WheelsMovement move(BP);
    Rotation rotation(BP);
    std::atomic<bool> stopFlag(false);

    // **Step 1: Too close to the right wall correction**
    if (rightSensor <= 6) {
        motor.setRightWheelDPS(-250);  // Right wheel moves faster
        motor.setLeftWheelDPS(-150);   // Left wheel moves slower
        usleep(1000000*1.2);  // Move for 1 second

        motor.setRightWheelDPS(-100);  // Right wheel slower
        motor.setLeftWheelDPS(-310);   // Left wheel faster
        usleep(500000);  // Move for 0.5 seconds

        motor.setRightWheelDPS(0);
        motor.setLeftWheelDPS(0);
    }

    if (frontSensor <= 6) {
        rotation.rotateBackwards(stopFlag);  // Perform 180° backward rotation
    } else {
        // **Step 3: Move forward until front sensor ≤ 6, then rotate**
        move.goForward();
        while (sensorObj.returnUltrasonicValue(1) > 6) {
            // Keep moving forward until front sensor reads ≤ 6
        }
        move.stop();
        rotation.rotateBackwards(stopFlag); // Perform 180° backward rotation
    }
}

void SpecialCases::cornerTrapLeft(Sensor &sensorObj, BrickPi3 &BP) {
    float leftSensor = sensorObj.returnUltrasonicValue(3); // Get left sensor data
    float frontSensor = sensorObj.returnUltrasonicValue(1); // Get front sensor data
    Motor motor(BP);
    WheelsMovement move(BP);
    Rotation rotation(BP);
    std::atomic<bool> stopFlag(false);

    // **Step 1: Too close to the left wall correction**
    if (leftSensor <= 6) {
        motor.setLeftWheelDPS(-250);   // Left wheel moves faster
        motor.setRightWheelDPS(-150);  // Right wheel moves slower
        usleep(1000000*1.2);  // Move for 1 second

        motor.setLeftWheelDPS(-100);   // Left wheel slower
        motor.setRightWheelDPS(-310);  // Right wheel faster
        usleep(500000);  // Move for 0.5 seconds

        motor.setLeftWheelDPS(0);
        motor.setRightWheelDPS(0);
    }

    if (frontSensor <= 6) {
        rotation.rotateBackwards(stopFlag);  // Perform 180° backward rotation
    } else {
        // **Step 3: Move forward until front sensor ≤ 6, then rotate**
        move.goForward();
        while (sensorObj.returnUltrasonicValue(1) > 6) {
            // Keep moving forward until front sensor reads ≤ 6
        }
        move.stop();
        rotation.rotateBackwards(stopFlag); // Perform 180° backward rotation
    }
}





