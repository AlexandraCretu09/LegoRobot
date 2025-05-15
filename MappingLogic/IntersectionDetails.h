//
// Created by alexa on 2/25/25.
//

#ifndef INTERSECTIONDETAILS_H
#define INTERSECTIONDETAILS_H

#include "../common.h"

using namespace std;
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
    string buildPathFromAToB(string a, string b);

    bool returnToLastIntersectionLogic();
    bool checkIfNodeIsRoot();
    string getID();
    direction getCurrentDirection();
    IntersectionWays getPossibleIntersectionWays();
private:
    void printAllNodesRecursive(intersectionNode* node);
    intersectionNode *currentNode;
    intersectionNode *getRoot(intersectionNode *node);
    IntersectionWays getIntersectionPossibleWays();

    static string getLowestCommonAncestor(const string &a, const string &b);
    static string buildPathFromAToCommonAncestor(string a, const string& commonAncestor);
    static bool compareStrings(const string& a, const string& b);
    static void getAtoBDifferentDirection(char &x, char &y, const string &a, const string &b);
    static char chooseDirectionFromAToBInCriticalPoint(const char &x, const char &y);
    static string buildPathFromCommonAncestorToB(string buildPath, string b);
};



#endif //INTERSECTIONDETAILS_H
