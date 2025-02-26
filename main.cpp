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
#include "MappingLogic/IntersectionDetails.h"
#include "MonitorGyroscope/GyroMonitor.h"

#include "Sensors/getSensorData.h"
#include "Motor/motor.h"
#include "Movement/rotation.h"
#include "Movement/wheelsMovement.h"
#include "PID/PID.h"
#include "SpecialCases/SpecialCases.h"
#include "MappingLogic/IntersectionDetails.h"

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

void printIntersectionResult(IntersectionCheckerResult result) {
	printf("Intersection result:\n");
	printf("Intersection on the right: %d\n", result.right );
	printf("Intersection on the left: %d\n", result.left );
	printf("Intersection on the forward: %d\n", result.forward );
	printf("Special case 1: %d\n", result.specialCase1);
	printf("Special case 2: %d\n", result.specialCase2);
}

void startMovement(atomic<bool> &stopFlag, atomic<bool> &checkerFlag,
		CheckForIntersection &checkerThread, BrickPi3 BP) {

	checkerThread.startMonitoring();
	PID pid(checkerFlag, BP);
	pid.correctPath(stopFlag, checkerFlag);
}

IntersectionCheckerResult rememberIntersection(atomic<bool> &stopFlag, CheckForIntersection &checkerThread, BrickPi3 BP) {
	checkerThread.stopMonitoring();
	IntersectionCheckerResult result = checkerThread.getLatestResult();
	//printIntersectionResult(result);
	return result;
}

IntersectionWays convertIntersectionWithSpecialCasesToOnlyWays(IntersectionCheckerResult fullResult){
	IntersectionWays result = {false, false, false};
	if (fullResult.left) result.left = true;
	if (fullResult.right) result.right = true;
	if (fullResult.forward)  result.forward = true;
	return result;
}

void addNewIntersectionToMap(IntersectionDetails &map, IntersectionCheckerResult result) {

	map.addNewIntersection(convertIntersectionWithSpecialCasesToOnlyWays(result));
	map.printAllNodes();
}

void chooseNextDirection(IntersectionDetails map) {
	printf("Next direction should be: %d\n\n", map.chooseNextDirection());
}

void testRobot(atomic<bool> &stopFlag,BrickPi3 BP){

	Motor motor(BP);
	WheelsMovement move(BP);
	Sensor sensor(BP);

	printf("Reset encoders\n");
	motor.resetBothMotorEncoders();

	usleep(second);
	IntersectionDetails map(sensor.returnGyroValue());

	atomic<bool> checkerFlag(false);
	bool ok = true;
	CheckForIntersection checkerThread(stopFlag, checkerFlag, BP);


	while (!stopFlag.load()) {
		if (ok) {
			startMovement(stopFlag, checkerFlag, checkerThread, BP);
			ok = false;
		}
		if (checkerFlag.load()) {
			IntersectionCheckerResult result = rememberIntersection(stopFlag, checkerThread, BP);
			addNewIntersectionToMap(map, result);
			chooseNextDirection(map);
			map.printCurrentNode();
			stopFlag.store(true);
		}
	}
	// for (int i=0; i<10;i ++) {
	// 	printf("sensor: %f\n",sensor.returnUltrasonicValue(2));
	// 	usleep(second);
	// }


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

	//BP.get_version_hardware(str);
	//printf("Hardware version: %s\n", str);

	//BP.get_version_firmware(str);
	//printf("Firmware version: %s\n", str);

	printf("Battery voltage : %.3f\n", BP.get_voltage_battery());
	printf("9v voltage      : %.3f\n", BP.get_voltage_9v());
	printf("5v voltage      : %.3f\n", BP.get_voltage_5v());
	printf("3.3v voltage    : %.3f\n", BP.get_voltage_3v3());


	BP.set_sensor_type(PORT_4, SENSOR_TYPE_NXT_ULTRASONIC); // left
	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC); // right
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_EV3_GYRO_ABS_DPS); //
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC); //



	atomic<bool> stopFlag(false);

	Sensor button(BP);
	thread killMonitorThread( monitorKillButton, ref(button), ref(stopFlag), ref(BP));


	testRobot(stopFlag, BP);

	killMonitorThread.join();

	printf("Program exiting.\n");
	return 0;
}
