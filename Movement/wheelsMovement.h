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
	void goForward(float noOfSeconds, BrickPi3 BP);
	void goForward(BrickPi3 BP);

	void goBackwards(float noOfSeconds, BrickPi3 BP);
	void goBackwards(BrickPi3 BP);

	void moveLeftWheel(float degrees, BrickPi3 BP);
	void moveRightWheel(float degrees, BrickPi3 BP);

	void stop(BrickPi3 BP);
	void checkIfStopped(BrickPi3 BP);

};



#endif /* MOVEMENT_WHEELSMOVEMENT_H_ */
