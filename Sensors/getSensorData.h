/*
 * getSensorData.h
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */

#ifndef GETSENSORDATA_H_
#define GETSENSORDATA_H_

#include <BrickPi3.h>
#include <atomic>

class Sensor{
	public:
		Sensor(BrickPi3 &BP);
		int getPortNumber(int x);
		float returnUltrasonicValue(int x);
		bool killButton();
		sensor_gyro_t returnGyroValue();
private:
	BrickPi3 &BP;
};

#endif /* GETSENSORDATA_H_ */
