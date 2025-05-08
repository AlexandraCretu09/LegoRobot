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
	explicit Rotation(BrickPi3 BP);
	void rotateRightForAuto( std::atomic<bool> &stopFlag);
	void rotateLeftForAuto(std::atomic<bool> &stopFlag);
	void rotateBackwardsForAuto(std::atomic<bool> &stopFlag);

	void rotateRightForManual();
	void rotateLeftForManual();
	void rotateBackwardsForManual();

private:
	BrickPi3 BP;
	void rotateRight(std::atomic<bool> &stopFlag);
	void rotateLeft(std::atomic<bool> &stopFlag);
	void rotateBackwards(std::atomic<bool> &stopFlag);
};



#endif /* MOVEMENT_ROTATION_H_ */
