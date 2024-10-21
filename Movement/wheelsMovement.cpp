/*
 * movement.cpp
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */




#include "wheelsMovement.h"

#include <iostream>
#include <unistd.h>


#include "BrickPi3.h"
#include "../Motor/motor.h"

using namespace std;

float second1 = 1000000.0;



void WheelsMovement::goForward(float noOfSeconds, BrickPi3 BP){

	float time = noOfSeconds * second1;
	Motor motor;

	motor.setLeftWheelDPS(250, BP);
	motor.setRightWheelDPS(250, BP);
	usleep(time);
}

void WheelsMovement::goForward(BrickPi3 BP){
	Motor motor;

	motor.setLeftWheelDPS(250, BP);
	motor.setRightWheelDPS(250, BP);
}

void WheelsMovement::goBackwards(float noOfSeconds, BrickPi3 BP){

	float time = noOfSeconds * second1;

	Motor motor;

	motor.setLeftWheelDPS(-200, BP);
	motor.setRightWheelDPS(-200, BP);
	usleep(time);
}

void WheelsMovement::goBackwards(BrickPi3 BP){
	Motor motor;

	motor.setLeftWheelDPS(-200, BP);
	motor.setRightWheelDPS(-200, BP);
}

void WheelsMovement::moveLeftWheel(float degrees, BrickPi3 BP){
	Motor motor;
	motor.setLeftWheelDPS(degrees, BP);
}

void WheelsMovement::moveRightWheel(float degrees, BrickPi3 BP){
	Motor motor;
	motor.setRightWheelDPS(degrees, BP);
}

void WheelsMovement::stop(BrickPi3 BP){
	Motor motor;

	motor.setLeftWheelDPS(0, BP);
	motor.setRightWheelDPS(0, BP);
}

void WheelsMovement::checkIfStopped(BrickPi3 BP){
	Motor motor;
}

















