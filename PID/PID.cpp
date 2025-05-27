/*
 * PID.cpp
 *
 *  Created on: 8 Aug 2024
 *      Author: alexa
 */


#include <iostream>
#include <time.h>
#include <atomic>
#include <unistd.h>

#include "BrickPi3.h"
#include "PID.h"
#include "../Sensors/getSensorData.h"
#include "../Motor/motor.h"
#include "../Movement/wheelsMovement.h"


using namespace std;

PID::PID(atomic<bool> &stopFlag, atomic<bool> &checkerFlag,  atomic<bool> &checkerForFrontBlock, BrickPi3 BP) : stopFlag(stopFlag), checkerFlag(checkerFlag),
	checkerForFrontBlock(checkerForFrontBlock), BP(BP) {
}


void PID::correctPath(){

	WheelsMovement move(BP);
	Sensor sensor(BP);
	int ct = 1;
	int ctLeft = 1, ctRight = 1;

	clock_t start = clock();

	while(!stopFlag.load() && !checkerFlag.load() && !checkerForFrontBlock.load()) {
		if(ct == 1) {
			move.goForward();
			ct = 0;
		}
		{
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
			constexpr float second = 1000000.0;
			usleep(second/4);
		}
	}


	clock_t now = clock();
	double elapsedTime = (double)(now - start) / CLOCKS_PER_SEC;

	move.stop();

	distanceTravelled = computeDistanceTravelled(elapsedTime);

}

bool PID::isTooCloseToLeft() {
	Sensor sensor(BP);

	if(sensor.returnUltrasonicValue(4) <= 7 ) {
		// printf("Left:\n");
		// printf("Left sensor: %f\n", sensor.returnUltrasonicValue(4));
		// printf("Right sensor: %f\n\n", sensor.returnUltrasonicValue(3));
		return true;
	}
	return false;
}

bool PID::isTooCloseToRight() {
	Sensor sensor(BP);
	if(sensor.returnUltrasonicValue(3) <= 7 ) {
		// printf("Right:\n");
		// printf("Left sensor: %f\n", sensor.returnUltrasonicValue(4));
		// printf("Right sensor: %f\n\n", sensor.returnUltrasonicValue(3));
		return true;
	}
	return false;
}

void PID::passTime(float seconds, atomic<bool>& stopFlag) {
	// printf("in passTime method\n");
	clock_t start = clock();
	while(clock() - start < seconds * CLOCKS_PER_SEC && !stopFlag.load()) {
		if(stopFlag.load())
			break;
	}
}

void PID::correctLeft(std::atomic<bool> &stopFlag) {

	if(stopFlag.load())
		return;
	WheelsMovement move(BP);
	// printf("in left correction\n");
	move.moveLeftWheel(255);
	move.moveRightWheel(245);
	passTime(2, stopFlag);
	// printf("time has passed for left\n");
	antiCorrection(false, stopFlag);
}

void PID::correctRight(std::atomic<bool> &stopFlag) {

	if(stopFlag.load())
		return;
	WheelsMovement move(BP);
	// printf("in right correction\n");
	move.moveRightWheel(260);
	move.moveLeftWheel(240);
	passTime(2, stopFlag);
	// printf("time has passed for right\n");
	antiCorrection(true, stopFlag);
}



void PID::antiCorrection(bool right, atomic<bool>& stopFlag) {
	WheelsMovement move(BP);
	if(stopFlag.load())
		return;
	if(right) {
		// printf("anticorrection on right\n");
		move.moveRightWheel(248);
		move.moveLeftWheel(252);
		passTime(0.5, stopFlag);
		move.goForward();
	}
	else {
		// printf("anticorrection on left\n");
		move.moveRightWheel(252);
		move.moveLeftWheel(248);
		passTime(0.5, stopFlag);
		move.goForward();
	}
}

double PID::computeDistanceTravelled(double elapsedTime) {
	double distancePerRotation = 16.0;
	double totalDegrees = 250 * elapsedTime;
	double wheelRotation = totalDegrees/360;
	return distancePerRotation * wheelRotation;

}







