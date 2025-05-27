/*
 * getSensorData.h
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */

#ifndef GETSENSORDATA_H_
#define GETSENSORDATA_H_

#include <BrickPi3.h>

class Sensor{
	public:
		explicit Sensor(BrickPi3 &BP);
		int getPortNumber(int x);
		float returnUltrasonicValue(int x);
		bool killButton();

		int16_t returnGyroValue();
private:
	BrickPi3 &BP;
};

#endif /* GETSENSORDATA_H_ */
