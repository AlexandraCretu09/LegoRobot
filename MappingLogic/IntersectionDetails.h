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
    void printAllNodesID();

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
    void printAllNodesIDPrivate(intersectionNode *node);

    static string getLowestCommonAncestor( string a,  string b);
    static string buildPathFromAToCommonAncestor(string a,  string commonAncestor);
    static bool compareStrings( string a,  string b);
    static void getAtoBDifferentDirection(char &x, char &y,  string a, string b);
    static char chooseDirectionFromAToBInCriticalPoint( char x,  char y);
    static string buildPathFromCommonAncestorToB(string buildPath, string commonAncestor, string b);
};



#endif //INTERSECTIONDETAILS_H
