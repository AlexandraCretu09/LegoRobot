//
// Created by alexa on 2/25/25.
//

#ifndef INTERSECTIONDETAILS_H
#define INTERSECTIONDETAILS_H



struct intersectionNode {

    intersectionNode *right;
    intersectionNode *left;
    intersectionNode *forward;
    intersectionNode *parent;
    double initialGyroscopeReading;
    direction currentDirection;

};



class IntersectionDetails {
public:
    IntersectionDetails(int gyroscopeReading);
    void addNewIntersection();
    void setNewDirection(bool right, bool left, bool forward);
    void printAllNodes();
private:
    intersectionNode *currentNode;
};



#endif //INTERSECTIONDETAILS_H
