/*
 * PID.cpp
 *
 *  Created on: 8 Aug 2024
 *      Author: alexa
 */


#include <iostream>
#include <time.h>
#include <atomic>

#include "BrickPi3.h"
#include "../Sensors/getSensorData.h"
#include "../Motor/motor.h"
#include "PID.h"
#include "../Movement/wheelsMovement.h"


using namespace std;

void PID::correctPath(float noOfSeconds, atomic<bool>& stopFlag, BrickPi3 BP){

	WheelsMovement move;
	Sensor sensors;

	time_t start, end;
	double timeElapsed;
	start =  clock();
	timeElapsed = 0.0;

	cout<<"entered PID, start:"<<start<<'\n';

	uint8_t StateB, StateC;
	int8_t PowerB, PowerC;
	int32_t PositionB, PositionC;
	int16_t DPSB, DPSC;



	while(noOfSeconds > timeElapsed || !stopFlag){

		BP.get_motor_status(PORT_A, StateB, PowerB, PositionB, DPSB);
		printf("State: %d, Power: %d, Position: %d, DPSB: %d\n", StateB, PowerB, PositionB, DPSB);

		move.goForward(BP);

		end = clock();
		timeElapsed = double(end - start)/ double(CLOCKS_PER_SEC);
		//cout<<"computed time: "<<timeElapsed<<"\n";
	}

	move.stop(BP);

}

