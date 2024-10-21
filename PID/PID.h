/*
 * PID.h
 *
 *  Created on: 8 Aug 2024
 *      Author: alexa
 */

#ifndef PID_PID_H_
#define PID_PID_H_

#include <BrickPi3.h>
#include <atomic>

class PID{
public:
	void correctPath(float noOfSeconds, std::atomic<bool>& stopFlag,  BrickPi3 BP);
};



#endif /* PID_PID_H_ */
