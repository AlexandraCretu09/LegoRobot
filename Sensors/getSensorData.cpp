/*
 * getSensorData.cpp
 *
 *  Created on: 26 Jul 2024
 *      Author: alexa
 */

#include <BrickPi3.h>
#include <iostream>
#include <mutex>

#include "getSensorData.h"
#include "../common.h"

using namespace std;

Sensor::Sensor(BrickPi3 &BP) : BP(BP) {}


int Sensor::getPortNumber(int x){
	switch(x){
	case 1:
		return PORT_1;
	case 2:
		return PORT_2;
	case 3:
		return PORT_3;
	case 4:
		return PORT_4;
	default:
		printf("Invalid port number");
		return -1;
	}
}


float Sensor::returnUltrasonicValue(int x){

	int PORT = getPortNumber(x);

	sensor_ultrasonic_t Ultra;

	lock_guard<mutex> lock(bpMutex);
	if(BP.get_sensor(PORT, &Ultra)){
		;
	}else{

		return Ultra.cm;
	}
	return NULL;
}

bool Sensor::killButton(){
	sensor_touch_t Button;

	lock_guard<mutex> lock(bpMutex);

	if(!BP.get_sensor(PORT_2, &Button)){
		return Button.pressed;
	}
	return false;
}

sensor_gyro_t Sensor::returnGyroValue(){
	sensor_gyro_t Gyro;

	lock_guard<mutex> lock(bpMutex);
	if(!BP.get_sensor(PORT_1, &Gyro)){
		return Gyro;
	}
}




