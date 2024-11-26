/*
 * rotation.h
 *
 *  Created on: 4 Sept 2024
 *      Author: alexa
 */

#ifndef MOVEMENT_ROTATION_H_
#define MOVEMENT_ROTATION_H_

#include <BrickPi3.h>
#include <atomic>

class Rotation{
public:
	Rotation(BrickPi3 BP);
	void rotateRight( std::atomic<bool> &stopFlag);
	void rotateLeft( std::atomic<bool> &stopFlag);
	void rotateBackwards(std::atomic<bool> &stopFlag);

private:
	BrickPi3 BP;
};



#endif /* MOVEMENT_ROTATION_H_ */
