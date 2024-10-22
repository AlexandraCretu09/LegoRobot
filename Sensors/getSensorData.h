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
		int getPortNumber(int x);
		float returnUltrasonicValue(int x,  BrickPi3 BP);
		bool killButton(BrickPi3 &BP);
		sensor_gyro_t returnGyroValue(BrickPi3 BP);

};

#endif /* GETSENSORDATA_H_ */
