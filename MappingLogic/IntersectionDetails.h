//
// Created by alexa on 2/25/25.
//

#ifndef INTERSECTIONDETAILS_H
#define INTERSECTIONDETAILS_H

#include "../common.h"

struct intersectionNode {
    intersectionNode *right;
    intersectionNode *left;
    intersectionNode *forward;
    intersectionNode *parent;
    double initialGyroscopeReading;
    direction currentDirection;
    IntersectionWays possibleIntersectionWays;
    IntersectionWays visitedIntersectionWays;
};



class IntersectionDetails {
public:
    IntersectionDetails(int gyroscopeReading);
    void addNewIntersection(IntersectionWays possibleIntersectionWays);
    void setNewDirection(bool right, bool left, bool forward);
    void printAllNodes();
    IntersectionWays getIntersectionWays();
private:
    intersectionNode *currentNode;
};



#endif //INTERSECTIONDETAILS_H
