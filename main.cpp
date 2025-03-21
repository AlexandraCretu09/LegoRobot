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
extern bool returnToLastIntersection;

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
	stopFlag.store(false);
	IntersectionCheckerResult result = checkerThread.getLatestResult();
	return result;
}

IntersectionWays convertIntersectionWithSpecialCasesToOnlyWays(IntersectionCheckerResult fullResult){
	IntersectionWays result = {false, false, false};
	if (fullResult.left) result.left = true;
	if (fullResult.right) result.right = true;
	if (fullResult.forward)  result.forward = true;
	return result;
}

void addNewIntersectionToMap(IntersectionDetails &map, IntersectionWays result) {

	map.addNewIntersection(result);
	map.printAllParentNodes();
}

void chooseNextDirection(IntersectionDetails map, atomic<bool> &stopFlag, BrickPi3 &BP) {
	turnDirection nextRotation = map.chooseNextDirection();
	printf("Next direction should be: %d\n\n", nextRotation);
	Rotation rotation(BP);

	if (!stopFlag.load()) {
		switch (nextRotation) {
			case turnRight:
				rotation.rotateRight(stopFlag);
				break;
			case turnLeft:
				rotation.rotateLeft(stopFlag);
				break;
			case goStraight:
				rotation.goStraight(stopFlag);
				break;
			case turnBackwards:
				printf("in choose next direction\n");
				rotation.rotateBackwards(stopFlag);
				break;
		}
	}
}

// void checkDeadend(atomic<bool> &stopFlag, IntersectionCheckerResult fullResult, BrickPi3 &BP) {
// 	printf("in checkDeadend method \n");
// 	Rotation rotate(BP);
// 	if (fullResult.deadend) {
// 		rotate.rotateBackwards(stopFlag);
// 	}
// }

void returnToLastIntersectionLogic(IntersectionDetails &map, atomic<bool> &stopFlag) {
	bool result = map.returnToLastIntersectionLogic();
	if (result == true) {
		printf("Scanned the whole labyrinth\n");
		stopFlag.store(true);
	}
}



void testRobot(atomic<bool> &stopFlag,BrickPi3 &BP){

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
		 // break;
		if (ok) {
			startMovement(stopFlag, checkerFlag, checkerThread, BP);
			ok = false;
		}
		if (checkerFlag.load()) {
			IntersectionCheckerResult fullResult = rememberIntersection(stopFlag, checkerThread, BP);
			IntersectionWays result = convertIntersectionWithSpecialCasesToOnlyWays(fullResult);
			if (!returnToLastIntersection) {
				addNewIntersectionToMap(map, result);
			}
			else {
				returnToLastIntersectionLogic(map, stopFlag);
			}
			chooseNextDirection(map, stopFlag, BP);
			// stopFlag.store(true);
		}
	}


	// Sensor sensorObj(BP);  // Create Sensor object
	// SpecialCases specialCases(BP);
	//
	// specialCases.cornerTrapLeft(sensorObj, BP); // Handle left case
	//
	// move.stop();
	// //gyroMonitor.stopMonitoring();
	// checkerThread.stopMonitoring();
	// stopFlag.store(true);



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
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC); // forward



	atomic<bool> stopFlag(false);

	Sensor button(BP);
	thread killMonitorThread( monitorKillButton, ref(button), ref(stopFlag), ref(BP));


	testRobot(stopFlag, BP);

	killMonitorThread.join();

	printf("Program exiting.\n");
	return 0;
}
