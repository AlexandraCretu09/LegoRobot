//
// Created by alexa on 2/25/25.
//

#ifndef INTERSECTIONDETAILS_H
#define INTERSECTIONDETAILS_H

#include "../common.h"

struct intersectionNode {
    std::string ID;
    intersectionNode *right;
    intersectionNode *left;
    intersectionNode *forward;
    intersectionNode *parent;
    direction currentDirection;
    IntersectionWays possibleIntersectionWays;
};



class IntersectionDetails {
public:
    IntersectionDetails();
    void addNewIntersection(IntersectionWays possibleIntersectionWays);
    void printAllParentNodes();
    void printAllNodes();
    void printCurrentNode();
    void printCurrentNode(intersectionNode* node);
    turnDirection chooseNextDirection();

    bool returnToLastIntersectionLogic();
    bool checkIfNodeIsRoot();
    std::string getID();
    direction getCurrentDirection();
    IntersectionWays getPossibleIntersectionWays();
private:
    void printAllNodesRecursive(intersectionNode* node);
    intersectionNode *currentNode;
    intersectionNode *getRoot(intersectionNode *node);
    IntersectionWays getIntersectionPossibleWays();
    IntersectionWays getIntersectionVisitedWays();
};



#endif //INTERSECTIONDETAILS_H
