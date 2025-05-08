/*
 * motor.h
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "../common.h"

#include <BrickPi3.h>
#include <iostream>
using namespace std;



class Motor{
	public:
		explicit Motor(BrickPi3 BP);
		void setRightWheelDPS(int degrees);
		void setLeftWheelDPS(int degrees);
		MotorDetails getRightMotorStatus();
		MotorDetails getLeftMotorStatus();
		void resetBothMotorEncoders();
	private:
		BrickPi3 BP;

};



#endif /* MOTOR_H_ */
