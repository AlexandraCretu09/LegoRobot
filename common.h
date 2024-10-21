/*
 * common.h
 *
 *  Created on: 5 Sept 2024
 *      Author: alexa
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <mutex>

extern std::mutex bpMutex;

struct MotorDetails{
	uint8_t State;
	int8_t Power;
	int32_t Position;
	int16_t DPS;
	int32_t Encoder;
};



#endif /* COMMON_H_ */
