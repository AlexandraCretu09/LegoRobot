/*
 * main.cpp
 *
 *  Created on: Mar 8, 2024
 *      Author: valy
 */

#include "brickpi/BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <pthread.h>

#include "common.h"

#include "Sensors/getSensorData.h"
#include "Motor/motor.h"
#include "Movement/rotation.h"
#include "Movement/wheelsMovement.h"
#include "PID/PID.h"

using namespace std;

extern mutex bpMutex;

float second = 1000000.0;
bool ok = false;

void monitorKillButton(Sensor& button, atomic<bool>& stopFlag, BrickPi3 &BP){
	while(!stopFlag){
		if(button.killButton(BP)){
			printf("Button pressed.\n");
			stopFlag = true;
		}
		usleep(250000);
	}
}

void testRobot(atomic<bool> &stopFlag,BrickPi3 BP){

	WheelsMovement move;
	Rotation rotate;//
	Motor motor;
	Sensor sensor;

	MotorDetails rightMotorDetails, leftMotorDetails;
	sensor_gyro_t gyroValues;

	int ct = 1;
	/*
	while(!stopFlag){
		gyroValues = sensor.returnGyroValue(BP);
		printf("Abs: %d\n", gyroValues.abs);
		//printf("DPS: %d\n", gyroValues.dps)
		usleep(second/2);
	}*/

	{
		lock_guard<mutex> lock(bpMutex);
		BP.reset_all();
	}
	stopFlag = true;




}

// 55 - diametrul pe dreapta

// o rotatie o face in 16 cm -> un metru = 6.25 rotatii



int main(void)
{
	BrickPi3 BP;

	BP.detect(); // Make sure that the BrickPi3 is communicating and that the firmware is compatible with the drivers.

	char str[33]; // Room for the 32-character serial number string plus the NULL terminator.

	BP.get_manufacturer(str);
	printf("Manufacturer    : %s\n", str);

	BP.get_board(str);
	printf("Board           : %s\n", str);

	BP.get_id(str);
	printf("Serial Number   : %s\n", str);

	BP.get_version_hardware(str);
	printf("Hardware version: %s\n", str);

	BP.get_version_firmware(str);
	printf("Firmware version: %s\n", str);

	printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
	printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
	printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
	printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());
	return 0;
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_NXT_ULTRASONIC); // forward
	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // left
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC); // right
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_EV3_GYRO_ABS_DPS); //



	atomic<bool> stopFlag(false);

	Sensor button;
	//thread killMonitorThread( monitorKillButton, ref(button), ref(stopFlag), ref(BP));



	testRobot(stopFlag, BP);

	//killMonitorThread.join();

	printf("Program exiting.\n");
	return 0;
}
