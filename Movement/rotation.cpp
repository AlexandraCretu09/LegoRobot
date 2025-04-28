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

	// printf("In rotate left\n");


	Motor motor(BP);
	MotorDetails motorDetails = {};
	WheelsMovement move(BP);
	bool ok = false;

	move.goBackwards(-200);

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
	// move.goForward(1.5);
	move.stop();
	//printf("finished rotate left\n");
}


void Rotation::rotateRight(atomic<bool> &stopFlag){

	//printf("In rotate right\n");
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
	// move.goForward(1.5);
	move.stop();
	//printf("finished rotate right\n");

}


void Rotation::rotateBackwards(std::atomic<bool> &stopFlag) {
	//printf("In rotate backwards\n");

	Motor motor(BP);
	MotorDetails motorDetailsA = {0};
	MotorDetails motorDetailsB = {0};
	WheelsMovement move(BP);
	bool ok = true;

	// printf("\n\n\n\n\n\n\nFirst while\n\n\n\n");
	while (motorDetailsB.Position > -450 && !stopFlag.load()) {
		if (ok) {
			move.goBackwards(-15);
			ok = false;
		}
		motorDetailsB = motor.getLeftMotorStatus();
		// printf("motor details: %d\n", motorDetailsB.Position);
	}
	move.stop();
	ok = true;
	motor.resetBothMotorEncoders();
	motorDetailsA = motor.getRightMotorStatus();
	// printf("\n\n\n\n\n\n\nSecond while\n\n\n\n");
	while (motorDetailsA.Position < 450 && !stopFlag.load()) {
		if (ok){
			move.moveRightWheel(200);
			ok = false;
		}
		motorDetailsA = motor.getRightMotorStatus();
		// printf("motor details: %d\n", motorDetailsA.Position);
	}

	move.stop();
	ok = true;
	motor.resetBothMotorEncoders();
	motorDetailsB = motor.getLeftMotorStatus();

	// printf("\n\n\n\n\n\n\nThird while\n\n\n\n");
	while (motorDetailsB.Position > -350 && !stopFlag.load()) {
		if (ok) {
			move.moveLeftWheel(-200);
			ok = false;
		}
		motorDetailsB = motor.getLeftMotorStatus();
		// printf("motor details: %d\n", motorDetailsB.Position);
	}

	move.stop();
	//printf("finished rotate backwards\n");
}


//
// void rotateBackwards(BrickPi3 BP){
// 	BP.reset_motor_encoder(PORT_B);
// 	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//
//
// 	printf("\nCase Backwards\n");
//
// 	while(PositionB > -550){
// 		if(killButton(BP))
// 			break;
// 		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
// 		printf("PoditionB: %d StateB: %d\n", PositionB, StateB);
// 		setRightWheelSpeed(-15, BP);
// 		setLeftWheelSpeed(-15, BP);
// 	}
//
// 	BP.reset_motor_encoder(PORT_B);
// 	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//
// 	while(PositionB < 450 || DPSB == 0){
// 		if(killButton(BP))
// 			break;
// 		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
// 		//printf("PoditionB: %d\n", PositionB);
// 		printf("1st While, DPS: %d\n", DPSB);
// 		setRightWheelSpeed(20, BP);
// 		setLeftWheelSpeed(0, BP);
// 	}
//
// 	BP.reset_motor_encoder(PORT_B);
// 	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
// 	BP.reset_motor_encoder(PORT_C);
// 	BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
// 	while(PositionC > -400 || DPSC == 0){
// 		if(killButton(BP))
// 			break;
// 		printf("2nd While\n");
// 		printf("PoditionC: %d\n", PositionC);
// 		BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
// 		setRightWheelSpeed(0, BP);
// 		setLeftWheelSpeed(-20, BP);
// 	}
//
// 	BP.reset_motor_encoder(PORT_B);
// 	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
// 	setRightWheelSpeed(0, BP);
// 	setLeftWheelSpeed(0, BP);
// }












