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
    PID(atomic<bool> &checkerFlag, BrickPi3 BP);

    void correctPath(std::atomic<bool> &stopFlag, atomic<bool> &checkerFlag);
private:
    BrickPi3 BP;
    atomic<bool> &checkerFlag;
    bool isTooCloseToLeft();
    bool isTooCloseToRight();
    void passTime(float seconds, std::atomic<bool>& stopFlag);
    void antiCorrection(bool right, std::atomic<bool>& stopFlag);
    void correctLeft(std::atomic<bool>& stopFlag);
    void correctRight(std::atomic<bool>& stopFlag);
};



#endif /* PID_PID_H_ */
