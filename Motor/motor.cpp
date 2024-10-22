/*
 * motor.cpp
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */

#include <iostream>
#include <string.h>


#include <BrickPi3.h>
#include "motor.h"
#include "../common.h"

using namespace std;

extern mutex bpMutex;



void Motor::setRightWheelDPS(int degrees, BrickPi3 BP){
	lock_guard<mutex> lock(bpMutex);
	BP.set_motor_dps(PORT_A, degrees);
}

void Motor::setLeftWheelDPS(int degrees, BrickPi3 BP){
	lock_guard<mutex> lock(bpMutex);
	BP.set_motor_dps(PORT_C, degrees);
}


MotorDetails Motor::getRightMotorStatus(BrickPi3 BP){


	MotorDetails motorDetails;

	lock_guard<mutex> lock(bpMutex); {
		BP.get_motor_status(PORT_A, motorDetails.State, motorDetails.Power, motorDetails.Position, motorDetails.DPS);
		BP.get_motor_encoder(PORT_A, motorDetails.Encoder);
	}
	return motorDetails;
}



MotorDetails Motor::getLeftMotorStatus(BrickPi3 BP){

	MotorDetails motorDetails;
	//printf("Locked motor for status.\n");
	lock_guard<mutex> lock(bpMutex);{
		BP.get_motor_status(PORT_C, motorDetails.State, motorDetails.Power, motorDetails.Position, motorDetails.DPS);
		BP.get_motor_encoder(PORT_C, motorDetails.Encoder);
	}
	return motorDetails;
}



void Motor::resetBothMotorEncoders(BrickPi3 BP){
	lock_guard<mutex> lock(bpMutex);
	BP.reset_motor_encoder(PORT_A);
	BP.reset_motor_encoder(PORT_C);
}




