/*
 * movement.cpp
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */




#include "wheelsMovement.h"

#include <iostream>
#include <unistd.h>
#include <ctime>

#include "BrickPi3.h"
#include "../Motor/motor.h"
#include "../Sensors/getSensorData.h"

using namespace std;

//float second1 = 1000000.0;


float second1 = 1000000.0;

WheelsMovement::WheelsMovement(BrickPi3 &BP) : BP(BP) {}


void WheelsMovement::goForward(float noOfSeconds){

	float time = noOfSeconds * second1;
	Motor motor(BP);

	motor.setLeftWheelDPS(250);
	motor.setRightWheelDPS(250);
	usleep(time);
}

void WheelsMovement::goForward(){
	Motor motor(BP);

	motor.setLeftWheelDPS(250);
	motor.setRightWheelDPS(250);
}

void WheelsMovement::goBackwards(float noOfSeconds){

	float time = noOfSeconds * second1;

	Motor motor(BP);

	motor.setLeftWheelDPS(-200);
	motor.setRightWheelDPS(-200);
	usleep(time);
}

void WheelsMovement::goBackwards(){
	Motor motor(BP);

	motor.setLeftWheelDPS(-200);
	motor.setRightWheelDPS(-200);
}

void WheelsMovement::moveLeftWheel(float degrees){
	Motor motor(BP);
	motor.setLeftWheelDPS(degrees);
}

void WheelsMovement::moveRightWheel(float degrees){
	Motor motor(BP);
	motor.setRightWheelDPS(degrees);
}

void WheelsMovement::stop(){
	Motor motor(BP);

	motor.setLeftWheelDPS(0);
	motor.setRightWheelDPS(0);
}


















