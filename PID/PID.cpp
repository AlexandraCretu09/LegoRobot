/*
 * PID.cpp
 *
 *  Created on: 8 Aug 2024
 *      Author: alexa
 */


#include <iostream>
#include <time.h>
#include <atomic>

#include "BrickPi3.h"
#include "../Sensors/getSensorData.h"
#include "../Motor/motor.h"
#include "PID.h"
#include "../Movement/wheelsMovement.h"
#include "../MonitorGyroscope/GyroMonitor.h"


using namespace std;

PID::PID(BrickPi3 BP) : BP(BP) {
}


bool PID::correctPath(atomic<bool> &stopFlag){

	GyroMonitor gyroMonitor(stopFlag, BP);
	WheelsMovement move(BP);
	int ct = 1;
	int ctLeft = 1, ctRight = 1;

	while(!stopFlag.load()) {
		if(ct == 1) {
			move.goForward();
			//gyroMonitor.startMonitoring();
			ct = 0;
		}
		{
			Sensor sensor(BP);
			if(sensor.returnUltrasonicValue(4) > 20 || sensor.returnUltrasonicValue(3) > 20)
				return true;

			if(ctLeft == 1) {
				if(isTooCloseToLeft()) {
					ctLeft = 0;
					ctRight = 1;
					correctLeft(stopFlag);
				}
			}
			if(ctRight == 1) {
				if(isTooCloseToRight()) {
					ctRight = 0;
					ctLeft = 1;
					correctRight(stopFlag);
				}
			}
		}
	}

	//gyroMonitor.stopMonitoring();
	move.stop();
}

bool PID::isTooCloseToLeft() {
	Sensor sensor(BP);

	if(sensor.returnUltrasonicValue(4) <= 7 ) {
		printf("Left:\n");
		printf("Left sensor: %f\n", sensor.returnUltrasonicValue(4));
		printf("Right sensor: %f\n\n", sensor.returnUltrasonicValue(3));
		return true;
	}
	return false;
}

bool PID::isTooCloseToRight() {
	Sensor sensor(BP);
	if(sensor.returnUltrasonicValue(3) <= 7 ) {
		printf("Right:\n");
		printf("Left sensor: %f\n", sensor.returnUltrasonicValue(4));
		printf("Right sensor: %f\n\n", sensor.returnUltrasonicValue(3));
		return true;
	}
	return false;
}

void PID::passTime(float seconds, atomic<bool>& stopFlag) {
	clock_t start = clock();
	while(clock() - start < seconds * CLOCKS_PER_SEC && !stopFlag.load()) {
		;
	}
}

void PID::correctLeft(std::atomic<bool> &stopFlag) {

	WheelsMovement move(BP);
	printf("in left correction\n");
	move.moveLeftWheel(255);
	move.moveRightWheel(245);
	passTime(2, stopFlag);
	printf("time has passed for left\n");
	antiCorrection(false, stopFlag);
}

void PID::correctRight(std::atomic<bool> &stopFlag) {

	WheelsMovement move(BP);
	printf("in right correction\n");
	move.moveRightWheel(260);
	move.moveLeftWheel(240);
	passTime(2, stopFlag);
	printf("time has passed for right\n");
	antiCorrection(true, stopFlag);
}



void PID::antiCorrection(bool right, atomic<bool>& stopFlag) {
	WheelsMovement move(BP);
	if(right) {
		printf("anticorrection on right\n");
		move.moveRightWheel(248);
		move.moveLeftWheel(252);
		passTime(0.5, stopFlag);
		move.goForward();
	}
	else {
		printf("anticorrection on left\n");
		move.moveRightWheel(252);
		move.moveLeftWheel(248);
		passTime(0.5, stopFlag);
		move.goForward();
	}
}




