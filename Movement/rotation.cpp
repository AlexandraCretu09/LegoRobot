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

using namespace std;



void Rotation::rotateLeft(atomic<bool> &stopFlag, BrickPi3 BP){


	Motor motor;
	MotorDetails motorDetails = {0};
	WheelsMovement move;
	bool ok = false;

	motor.resetMotorEncoder(BP);

	while((motorDetails.Position < 450)){
		if(!ok){
			move.moveRightWheel(200, BP);
			move.moveLeftWheel(20, BP);
			ok = true;
		}
		motorDetails = motor.getRightMotorStatus(BP);
		if(stopFlag)
			break;

		}
	move.stop(BP);
}


void Rotation::rotateRight(atomic<bool> &stopFlag, BrickPi3 BP){


	Motor motor;
	MotorDetails motorDetails = {0};
	WheelsMovement move;
	bool ok = false;

	motor.resetMotorEncoder(BP);

	while((motorDetails.Position < 450)){
		if(!ok){
			move.moveRightWheel(20, BP);
			move.moveLeftWheel(200, BP);
			ok = true;
		}
		motorDetails = motor.getLeftMotorStatus(BP);
		if(stopFlag)
			break;

		}
	move.stop(BP);
}











