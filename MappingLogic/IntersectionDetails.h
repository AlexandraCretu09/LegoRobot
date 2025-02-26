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
    void printAllNodes();
    void printCurrentNode();
    turnDirection chooseNextDirection();
    direction getCurrentDirection();
private:
    intersectionNode *currentNode;
    IntersectionWays getIntersectionPossibleWays();
    IntersectionWays getIntersectionVisitedWays();
};



#endif //INTERSECTIONDETAILS_H
