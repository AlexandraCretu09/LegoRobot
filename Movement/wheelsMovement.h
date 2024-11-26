/*
 * movement.h
 *
 *  Created on: 8 Aug 2024
 *      Author: alexa
 */

#ifndef MOVEMENT_WHEELSMOVEMENT_H_
#define MOVEMENT_WHEELSMOVEMENT_H_

#include <BrickPi3.h>

class WheelsMovement{
public:
	WheelsMovement(BrickPi3 &BP);
	void goForward(float noOfSeconds);
	void goForward();

	void goBackwards(float noOfSeconds);
	void goBackwards();

	void moveLeftWheel(float degrees);
	void moveRightWheel(float degrees);

	void stop();
	void checkIfStopped();
private:
	BrickPi3 BP;

};



#endif /* MOVEMENT_WHEELSMOVEMENT_H_ */
