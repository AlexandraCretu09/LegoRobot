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
    SpecialCases(BrickPi3 &BP);
    void toCloseToTheRight();
    void toCloseToTheLeft();
    void frontBlocked(Sensor &sensorObj);
    void cornerTrapRight(Sensor &sensorObj);
    void cornerTrapLeft(Sensor &sensorObj);

private:
    BrickPi3 &BP;
};



#endif //SPECIALCASES_H
