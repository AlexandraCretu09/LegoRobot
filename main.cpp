/*
 * main.cpp
 *
 *  Created on: Mar 8, 2024
 *      Author: valy
 */

#include "brickpi/BrickPi3.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <thread>
#include <atomic>


#include "common.h"
#include "CheckForIntersection/CheckForIntersection.h"
#include "FileProcessing/FileProcessing.h"
#include "MappingLogic/IntersectionDetails.h"
#include "MonitorGyroscope/MonitorIfStuck.h"

#include "Sensors/getSensorData.h"
#include "Motor/motor.h"
#include "Movement/rotation.h"
#include "Movement/wheelsMovement.h"
#include "PID/PID.h"
#include "SpecialCases/SpecialCases.h"

using namespace std;

// extern mutex bpMutex;
// extern bool returnToLastIntersection;
// extern double distanceTravelled;

bool ok = false;
float second = 1000000.0;

bool manualControl(BrickPi3 &BP);
bool checkForAndExecuteSpecialCases(deadendAndIntersectionSpecialCases fullResult, BrickPi3 &BP);

void monitorKillButton(Sensor& button, atomic<bool>& stopFlag, BrickPi3 &BP){
	while(!stopFlag.load()){
		if(button.killButton()){
			printf("Button pressed.\n");
			stopFlag = true;
		}
		usleep(250000);
	}
}

void printIntersectionResult(IntersectionCheckerResult result) {
	printf("Intersection result:\n");
	printf("Intersection on the right: %d\n", result.right );
	printf("Intersection on the left: %d\n", result.left );
	printf("Intersection on the forward: %d\n", result.forward );
	// printf("Special case 1 left: %d\n", result.specialCase1Left);
	// printf("Special case 1 right: %d\n", result.specialCase1Right);
	// printf("Special case 2 left: %d\n", result.specialCase2Left);
	// printf("Special case 2 right: %d\n", result.specialCase2Right);
}

void startMovement(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, CheckForIntersection &checkerThread, atomic<bool> &checkerForFrontBlock,
	MonitorIfStuck &monitorIfStuckThread, BrickPi3 BP) {

	checkerThread.startMonitoring();
	monitorIfStuckThread.startMonitoring();
	PID pid(stopFlag, checkerFlag, checkerForFrontBlock, BP);
	pid.correctPath();
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
	// map.printAllParentNodes();
}

bool chooseNextDirection(IntersectionDetails &map, atomic<bool> &stopFlag, BrickPi3 &BP) {
	turnDirection nextRotation = map.chooseNextDirection();
	Rotation rotation(BP);

	switch (nextRotation) {
		case turnRight:
			rotation.rotateRightForAuto(stopFlag);
			// checkerThread.checkUntilRobotPassedIntersection();
			return true;
		case turnLeft:
			rotation.rotateLeftForAuto(stopFlag);
			// checkerThread.checkUntilRobotPassedIntersection();
			return true;
		case goStraight:
			// checkerThread.checkUntilRobotPassedIntersection();
			return true;
		case turnBackwards:
			rotation.rotateBackwardsForAuto(stopFlag);
			return false;
	}
	return false;
}

void chooseNextDirectionAndExecuteSpecialCases(IntersectionDetails &map, atomic<bool> &stopFlag, CheckForIntersection &checkerThread, BrickPi3 &BP) {
	bool chooseNextDirectionResults = chooseNextDirection(map, stopFlag, BP);
	if (chooseNextDirectionResults) {
		int leftValues[] = {99, 99, 99}, rightValues[] = {99, 99, 99};
		while (true) {
			checkerThread.checkUntilRobotPassedIntersection(leftValues, rightValues);
			deadendAndIntersectionSpecialCases intersectionResult = checkerThread.checkIfIntersectionPositionIsInASpecialCase(leftValues, rightValues);
			if (!checkForAndExecuteSpecialCases(intersectionResult, BP)) {
				break;
			}
		}
	}
}

bool returnToLastIntersectionLogic(IntersectionDetails &map, atomic<bool> &stopFlag) {
	map.returnToLastIntersectionLogic();
	bool result = map.checkIfNodeIsRoot();
	if (result == true) {
		printf("Scanned the whole labyrinth\n");
		stopFlag.store(true);
		return true;
	}
	return false;
}

bool checkForAndExecuteSpecialCases(deadendAndIntersectionSpecialCases fullResult, BrickPi3 &BP) {
	SpecialCases specialCasesProceeder(BP);
	if (fullResult.tooCloseToTheLeftWall) {
		specialCasesProceeder.toCloseToTheLeft();
		return true;
	}if (fullResult.tooCloseToTheRightWall) {
		specialCasesProceeder.toCloseToTheRight();
		return true;
	}
	return false;
}

bool checkForAndExecuteSpecialCases(IntersectionCheckerResult fullResult, BrickPi3 &BP) {
	SpecialCases specialCasesProceeder(BP);
	if (fullResult.specialCase1Left) {
		specialCasesProceeder.toCloseToTheLeft();
		return true;
	}if (fullResult.specialCase1Right) {
		specialCasesProceeder.toCloseToTheRight();
		return true;
	}if (fullResult.specialCase2Left) {
		specialCasesProceeder.toCloseToTheLeft();
		return true;
	}if (fullResult.specialCase2Right) {
		specialCasesProceeder.toCloseToTheRight();
		return true;
	}
	return false;
}

void proceedWithSpecialCases(IntersectionCheckerResult fullResult, CheckForIntersection &checkerThread,BrickPi3 &BP) {
	if (fullResult.deadend) {
		WheelsMovement move(BP);
		printf("Realised its in a deadend\n");
		while (true){
			deadendAndIntersectionSpecialCases deadendResult = checkerThread.checkIfDeadendPositionIsInASpecialCase();
			fullResult.specialCase1Left = deadendResult.tooCloseToTheLeftWall;
			fullResult.specialCase1Right = deadendResult.tooCloseToTheRightWall;
			// printf("results: Left: %d, right: %d\n", fullResult.specialCase1Left, fullResult.specialCase1Right);

			if (!checkForAndExecuteSpecialCases(fullResult, BP)) {
				move.goForward(float(1.5));
				break;
			}
			move.goForward(float(1.5));
		}
	} else if (fullResult.specialCase1Left || fullResult.specialCase1Right || fullResult.specialCase2Left || fullResult.specialCase2Right)
	{
		printf("Realised its in a special case\n");
		checkForAndExecuteSpecialCases(fullResult, BP);
		checkerThread.checkUntilRobotReachIntersectionAgain();
	}
}


void autonomousMazeExploration(atomic<bool> &stopFlag,BrickPi3 &BP){

	bool restart;

	do {
		restart = false;
		Motor motor(BP);
		WheelsMovement move(BP);

		// printf("Test gyro: %f\n", sensor.returnGyroValue());

		printf("Reset encoders\n");
		motor.resetBothMotorEncoders();

		usleep(second);
		IntersectionDetails map{};

		atomic<bool> checkerFlag(false);
		atomic<bool> checkerForFrontBlock(false);
		atomic<bool> waiterForIntersectionResult(false);
		atomic<bool> readFromFileIfSwitchFromAutoToManual(false);

		bool okStartPID = true;
		bool countStop = false;
		bool stopIntersectionCheckerThread = false;

		FileProcessing fileProcessing(readFromFileIfSwitchFromAutoToManual, stopFlag);
		fileProcessing.writeToFileNewIntersection(map, 0);
		fileProcessing.startMonitoring();

		CheckForIntersection checkerThread(stopFlag, checkerFlag,waiterForIntersectionResult, BP);
		MonitorIfStuck monitorIfStuckThread(stopFlag, checkerForFrontBlock, BP);

		while (!stopFlag.load() && !readFromFileIfSwitchFromAutoToManual.load()) {
			// break;
			if (okStartPID) {
				printf("Start moving\n");
				waiterForIntersectionResult.store(false);
				startMovement(stopFlag, checkerFlag, checkerThread, checkerForFrontBlock, monitorIfStuckThread, BP);
				okStartPID = false;
			}
			monitorIfStuckThread.stopMonitoring();
			if (checkerForFrontBlock.load()) {

				// printf("Entered front end blockage\n");
				move.goBackwards(-250);
				checkerForFrontBlock.store(false);
				stopFlag.store(false);
				stopIntersectionCheckerThread = true;

			}
			if (checkerFlag.load()) {
				// printf("Exited front end blockage\n");
				while (waiterForIntersectionResult.load() == false) {
					if (!countStop) {
						move.stop();
						countStop = true;
					}
				}
				IntersectionCheckerResult fullResult = rememberIntersection(stopFlag, checkerThread, BP);
				proceedWithSpecialCases(fullResult, checkerThread, BP);
				IntersectionWays result = convertIntersectionWithSpecialCasesToOnlyWays(fullResult);
				printIntersectionResult(fullResult);

				bool rememberIfReturnToLastIntersection = returnToLastIntersection;

				if (!returnToLastIntersection) {
					printf("\nAdding a new intersection..\n");
					addNewIntersectionToMap(map, result);


				}
				else {
					printf("Returning to last intersection..\n");
					bool finishedLabyrinth = returnToLastIntersectionLogic(map, stopFlag);
					// map.printCurrentNode();
					if (finishedLabyrinth == true) {
						// fileProcessing.writeToFileFinishedLabyrinth();
						break;
					}

				}
				chooseNextDirectionAndExecuteSpecialCases(map, stopFlag,checkerThread, BP);
				if (!rememberIfReturnToLastIntersection)
					fileProcessing.writeToFileNewIntersection(map, distanceTravelled);
				else
					fileProcessing.writeToFileReturningToLastIntersection(map);

				// break;
				okStartPID = true;
				countStop = false;
				checkerFlag.store(false);
				stopIntersectionCheckerThread = false;
			}
			if (stopIntersectionCheckerThread) {
				checkerThread.stopMonitoring();
				okStartPID = true;
			}
		}

		move.stop();
		checkerThread.stopMonitoring();
		monitorIfStuckThread.stopMonitoring();

		map.printAllNodesID();
		// string b = "Rt_FR";
		// string a = "Rt_FRRLL";
		// printf("Path from A to B: %s\n\n", map.buildPathFromAToB(a, b).c_str());

		if (readFromFileIfSwitchFromAutoToManual.load() == true) {
			printf("Robot has stopped from auto and is switching to manual now.\n");
			fileProcessing.stopMonitoring();
			if (manualControl(BP)) {
				restart = true;
				stopFlag.store(false);
				usleep(second*3);
			}
		}else
			fileProcessing.stopMonitoringAndClosePipe();
	}while (restart);


	// map.printAllNodes();
	stopFlag.store(true);
}

void startThreadsForAutonomousExploration(BrickPi3 &BP) {
	atomic<bool> stopFlag(false);

	Sensor button(BP);
	thread killMonitorThread( monitorKillButton, ref(button), ref(stopFlag), ref(BP));

	autonomousMazeExploration(stopFlag, BP);

	killMonitorThread.join();
	printf("Program exiting.\n");
}

bool manualControl(BrickPi3 &BP) {
	FileProcessing fileProcessing;
	WheelsMovement movement(BP);
	Rotation rotation(BP);

	string message;

	while (true) {
		char command = fileProcessing.readFromFileOneLetterCommand();
		if (command == 'w' || command == 'W') {
			movement.goForward(250);
			continue;
		}
		if (command == 's' || command == 'S') {
			movement.goBackwards(-250);
			continue;
		}
		if (command == 'a' || command == 'A') {
			rotation.rotateLeftForManual();
			continue;
		}
		if (command == 'd' || command == 'D') {
			rotation.rotateRightForManual();
			continue;
		}
		if (command == 'b' || command == 'B') {
			rotation.rotateBackwardsForManual();
			continue;
		}

		if (command == 'l' || command == 'L')
			return true;
		if (command == 'x' || command == 'X')
			return false;
		if (command == '0')
			return false;

		message = "Invalid letter, please choose between the following:\n"
			"Go forward: W\n"
			"Go backwards: S\n"
			"Rotate right: D\n"
			"Rotate left: A\n"
			"Rotate backwards: B\n"
			"Exit: X\n";

		printf("%s",message.c_str());

		 // fileProcessing.writeToFileMessage(message);

	}

}


// 55 - diametrul pe dreapta

// o rotatie o face in 16 cm -> un metru = 6.25 rotatii




int main() {
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
	BP.set_sensor_type(PORT_1, SENSOR_TYPE_EV3_GYRO_ABS); //
	BP.set_sensor_type(PORT_2, SENSOR_TYPE_NXT_ULTRASONIC); // forward

	// Rotation rotate(BP);
	// atomic<bool> stopFlag;
	// stopFlag.store(false);
	// rotate.rotateBackwardsForAuto(stopFlag);
	// return 0;

	FileProcessing fileProcessing;

	while (true) {
		char result = fileProcessing.readFromFileOneLetterCommand();
		// char result = 'a';
		if ( result == 'm' ) {
			string message = "Started manual control of the robot\n";
			printf(message.c_str());
			// fileProcessing.writeToFileMessage(message);
			if (manualControl(BP) == true)
				result = 'a';
			else
				break;
		}
		if ( result == 'a' ) {
			string message = "Started autonomous exploration\n";
			printf(message.c_str());
			// fileProcessing.writeToFileMessage(message);
			startThreadsForAutonomousExploration(BP);
			break;
		}
		if (result == '0') {
			string message = "An error has occurred\n";
			printf(message.c_str());
			// fileProcessing.writeToFileMessage(message);
			break;
		}
		string message = "Invalid letter, please choose between Auto: 'a', or Manual: 'm'\n";
		printf(message.c_str());
		// fileProcessing.writeToFileMessage(message);
	}
	return 0;
}
