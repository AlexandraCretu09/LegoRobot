//
// Created by alexa on 04/12/24.
//

#ifndef SPECIALCASES_H
#define SPECIALCASES_H

#include "BrickPi3.h"
#include "../Sensors/getSensorData.h"

using namespace std;

class SpecialCases {
public:
    SpecialCases(BrickPi3 BP);
    void toCloseToTheRight(Sensor &sensorObj,BrickPi3 &BP);
    void toCloseToTheLeft(Sensor &sensorObj,BrickPi3 &BP);
    void frontBlocked(Sensor &sensorObj,BrickPi3 &BP);
    void cornerTrapRight(Sensor &sensorObj, BrickPi3 &BP);
    void cornerTrapLeft(Sensor &sensorObj, BrickPi3 &BP);

private:
    BrickPi3 BP;
};



#endif //SPECIALCASES_H
