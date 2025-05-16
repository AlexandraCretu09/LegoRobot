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


void Rotation::rotateLeftForAuto(atomic<bool> &stopFlag){

	Motor motor(BP);
	WheelsMovement move(BP);

	move.goBackwards(-200);
	motor.resetBothMotorEncoders();

	rotateLeft(stopFlag);
}

void Rotation::rotateLeftForManual(){
	atomic<bool> mimicStopFlag(false);
	rotateLeft(mimicStopFlag);
}


void Rotation::rotateRightForAuto(atomic<bool> &stopFlag){

	WheelsMovement move(BP);
	move.goBackwards(-200);
	rotateRight(stopFlag);
}

void Rotation::rotateRightForManual(){
	atomic<bool> mimicStopFlag(false);
	rotateRight(mimicStopFlag);
}


void Rotation::rotateBackwardsForAuto(std::atomic<bool> &stopFlag) {

	WheelsMovement move(BP);
	move.goBackwards(-470);
	rotateBackwards(stopFlag);
}

void Rotation::rotateBackwardsForManual() {
	atomic<bool> mimicStopFlag(false);
	rotateBackwards(mimicStopFlag);
}


void Rotation::rotateLeft(std::atomic<bool> &stopFlag) {
	Motor motor(BP);
	MotorDetails motorDetails = {};
	WheelsMovement move(BP);
	bool ok = true;
	while((motorDetails.Position < 450)){
		if(ok){
			move.moveRightWheel(200);
			move.moveLeftWheel(20);
			ok = false;
		}
		motorDetails = motor.getRightMotorStatus();
		if(stopFlag)
			break;
	}
	move.stop();
}

void Rotation::rotateRight(std::atomic<bool> &stopFlag) {
	Motor motor(BP);
	MotorDetails motorDetails = {0};
	WheelsMovement move(BP);
	bool ok = true;
	motor.resetBothMotorEncoders();
	while((motorDetails.Position < 450)){
		if(ok){
			move.moveRightWheel(40);
			move.moveLeftWheel(200);
			ok = false;
		}
		motorDetails = motor.getLeftMotorStatus();
		if(stopFlag)
			break;
	}
	move.stop();
}

void Rotation::rotateBackwards(std::atomic<bool> &stopFlag) {
	Motor motor(BP);
	MotorDetails motorDetailsA = {0};
	MotorDetails motorDetailsB = {0};
	WheelsMovement move(BP);

	bool ok = true;
	motor.resetBothMotorEncoders();
	motorDetailsA = motor.getRightMotorStatus();
	while (motorDetailsA.Position < 450 && !stopFlag.load()) {
		if (ok){
			move.moveRightWheel(200);
			ok = false;
		}
		motorDetailsA = motor.getRightMotorStatus();
	}
	move.stop();

	ok = true;
	motor.resetBothMotorEncoders();
	motorDetailsB = motor.getLeftMotorStatus();

	while (motorDetailsB.Position > -390 && !stopFlag.load()) {
		if (ok) {
			move.moveLeftWheel(-200);
			ok = false;
		}
		motorDetailsB = motor.getLeftMotorStatus();
	}
	move.stop();
}















