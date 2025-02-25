/*
 * rotation.cpp
 *
 *  Created on: 4 Sept 2024
 *      Author: alexa
 */


#include <BrickPi3.h>
#include <iostream>
#include <atomic>

#include "rotation.h"
#include "../Motor/motor.h"
#include "wheelsMovement.h"
#include "../common.h"
#include "../Sensors/getSensorData.h"

using namespace std;

Rotation::Rotation(BrickPi3 BP) : BP(BP) {}


void Rotation::rotateLeft(atomic<bool> &stopFlag){


	Motor motor(BP);
	MotorDetails motorDetails = {};
	WheelsMovement move(BP);
	bool ok = false;

	move.goBackwards(-150);

	motor.resetBothMotorEncoders();

	while((motorDetails.Position < 450)){
		if(!ok){
			move.moveRightWheel(200);
			move.moveLeftWheel(20);
			ok = true;
		}
		Sensor sensor(BP);
		double gyroValues = sensor.returnGyroValue();
		// printf("Abs: %d\n", gyroValues.abs);

		motorDetails = motor.getRightMotorStatus();

		if(stopFlag)
			break;
		}
	move.goForward(1.5);
	move.stop();
}


void Rotation::rotateRight(atomic<bool> &stopFlag){


	Motor motor(BP);
	MotorDetails motorDetails = {0};
	WheelsMovement move(BP);
	bool ok = false;


	move.goBackwards(-150);

	motor.resetBothMotorEncoders();

	while((motorDetails.Position < 450)){
		//printf("Position: %d\n", motorDetails.Position);
		if(!ok){

			move.moveRightWheel(20);
			move.moveLeftWheel(200);
			ok = true;
		}

		Sensor sensor(BP);
		double gyroValues = sensor.returnGyroValue();
		//printf("Abs: %d\n", gyroValues.abs);

		motorDetails = motor.getLeftMotorStatus();
		if(stopFlag)
			break;
		}
	move.goForward(1.5);
	move.stop();
}











