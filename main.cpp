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


#include "common.h"
#include "CheckForIntersection/CheckForIntersection.h"
#include "MonitorGyroscope/GyroMonitor.h"

#include "Sensors/getSensorData.h"
#include "Motor/motor.h"
#include "Movement/rotation.h"
#include "Movement/wheelsMovement.h"
#include "PID/PID.h"

using namespace std;

extern mutex bpMutex;


bool ok = false;
float second = 1000000.0;
void monitorKillButton(Sensor& button, atomic<bool>& stopFlag, BrickPi3 &BP){
	while(!stopFlag.load()){
		if(button.killButton()){
			printf("Button pressed.\n");
			stopFlag = true;
		}
		usleep(250000);
	}
}

// void monitorGyroscope(atomic<bool>& stopFlag, BrickPi3 &BP) {
// 	Sensor sensor;
// 	while(!stopFlag.load()) {
//
// 		sensor_gyro_t gyroValues = sensor.returnGyroValue(BP);
// 		printf("Abs: %d\n", gyroValues.abs);
// 	}
// }

void testRobot(atomic<bool> &stopFlag,BrickPi3 BP) {



	Motor motor(BP);
	WheelsMovement move(BP);

	// move.stop();
	// return;

	printf("Reset encoders\n");
	motor.resetBothMotorEncoders();

	usleep(second);

	atomic<bool> checkerFlag(false);
	int ok = 1;

	while(!stopFlag.load()) {
		CheckForIntersection checkerThread(stopFlag, checkerFlag, BP);
		if(ok == 1){
			checkerThread.startMonitoring();
			PID pid(checkerFlag, BP);
			pid.correctPath(stopFlag, checkerFlag);
			ok = 0;
		}
		if(checkerFlag.load()) {
			checkerThread.stopMonitoring();

			Sensor sensor(BP);
			Rotation rotate(BP);
			if(sensor.returnUltrasonicValue(4) > 20)
				rotate.rotateLeft(stopFlag);
			if(sensor.returnUltrasonicValue(3) > 20)
				rotate.rotateRight(stopFlag);
			checkerFlag.store(false);
			ok = 1;
		}
		if(stopFlag.load()) {
			checkerThread.stopMonitoring();
		}



	}

	move.stop();
	//gyroMonitor.stopMonitoring();

	stopFlag.store(true);
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


	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // left
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC); // right
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_EV3_GYRO_ABS_DPS); //
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_TOUCH); //



	atomic<bool> stopFlag(false);

	Sensor button(BP);
	thread killMonitorThread( monitorKillButton, ref(button), ref(stopFlag), ref(BP));


	testRobot(stopFlag, BP);

	killMonitorThread.join();

	printf("Program exiting.\n");
	return 0;
}
