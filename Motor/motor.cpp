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

Motor::Motor(BrickPi3 BP) : BP(BP) {

}


void Motor::setRightWheelDPS(int degrees){
	lock_guard<mutex> lock(bpMutex);
	BP.set_motor_dps(PORT_A, degrees);
}

void Motor::setLeftWheelDPS(int degrees){
	lock_guard<mutex> lock(bpMutex);
	BP.set_motor_dps(PORT_B, degrees);
}


MotorDetails Motor::getRightMotorStatus(){


	MotorDetails motorDetails;

	lock_guard<mutex> lock(bpMutex); {
		BP.get_motor_status(PORT_A, motorDetails.State, motorDetails.Power, motorDetails.Position, motorDetails.DPS);
		BP.get_motor_encoder(PORT_A, motorDetails.Encoder);
	}
	return motorDetails;
}



MotorDetails Motor::getLeftMotorStatus(){

	MotorDetails motorDetails;
	//printf("Locked motor for status.\n");
	lock_guard<mutex> lock(bpMutex);{
		BP.get_motor_status(PORT_B, motorDetails.State, motorDetails.Power, motorDetails.Position, motorDetails.DPS);
		BP.get_motor_encoder(PORT_B, motorDetails.Encoder);
	}
	return motorDetails;
}



void Motor::resetBothMotorEncoders(){
	lock_guard<mutex> lock(bpMutex);
	BP.reset_motor_encoder(PORT_A);
	BP.reset_motor_encoder(PORT_B);
}




