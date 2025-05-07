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
#include <deque>

using namespace std;

class PID{
public:
    PID(atomic<bool> &stopFlag, atomic<bool> &checkerFlag, atomic<bool> &checkerForFrontBlock, BrickPi3 BP);

    void correctPath();
private:
    BrickPi3 BP;
    atomic<bool> &stopFlag;
    atomic<bool> &checkerFlag;
    atomic<bool> &checkerForFrontBlock;
    static constexpr size_t BUFFER_SIZE = 5;

    double computeDistanceTravelled(double elapsedTime);
    bool isTooCloseToLeft();
    bool isTooCloseToRight();
    void passTime(float seconds, std::atomic<bool>& stopFlag);
    void antiCorrection(bool right, std::atomic<bool>& stopFlag);
    void correctLeft(std::atomic<bool>& stopFlag);
    void correctRight(std::atomic<bool>& stopFlag);
};



#endif /* PID_PID_H_ */
