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
		void setRightWheelDPS(int degrees, BrickPi3 BP);
		void setLeftWheelDPS(int degrees, BrickPi3 BP);
		MotorDetails getRightMotorStatus(BrickPi3 BP);
		MotorDetails getLeftMotorStatus(BrickPi3 BP);
		void resetBothMotorEncoders(BrickPi3 BP);

};



#endif /* MOTOR_H_ */
