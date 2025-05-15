//
// Created by alexa on 2/25/25.
//
#include <iostream>
#include <string.h>
#include "../common.h"
#include "IntersectionDetails.h"
using namespace std;

IntersectionDetails::IntersectionDetails() {
    intersectionNode *root = new intersectionNode();
    root->ID = "Rt_";
    root->forward = nullptr;
    root->left = nullptr;
    root->right = nullptr;
    root->parent = nullptr;
    root->currentDirection = direction::forward;
    root->possibleIntersectionWays = {false, false, true};
    currentNode = root;
}

void IntersectionDetails::addNewIntersection(IntersectionWays possibleIntersectionWays) {
    intersectionNode *newNode = new intersectionNode();
    newNode->parent = currentNode;
    newNode->forward = nullptr;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->currentDirection = direction::nothing;

    switch (currentNode->currentDirection) {
        case direction::forward:
            currentNode->forward = newNode;
            newNode->ID=currentNode->ID+"F";
            break;
        case direction::right:
            currentNode->right = newNode;
            newNode->ID=currentNode->ID+"R";
            break;
        case direction::left:
            currentNode->left = newNode;
            newNode->ID=currentNode->ID+"L";
            break;
    }

    if (possibleIntersectionWays.forward)
        newNode->possibleIntersectionWays.forward = true;
    if (possibleIntersectionWays.right)
        newNode->possibleIntersectionWays.right = true;
    if (possibleIntersectionWays.left)
        newNode->possibleIntersectionWays.left = true;

    currentNode = newNode;
}

void IntersectionDetails::printAllParentNodes() {
    intersectionNode *copy = currentNode;
    int x = 1;
    do {
        // printf("\nChild nodes of node %d\n", x);
        // printf("to the right: %d\n", copy->right);
        // printf("to the left: %d\n", copy->left);
        // printf("forward: %d\n", copy->forward);
        printf("\nPossible directions of node%d\n", x);
        printf("to the right: %d\n", copy->possibleIntersectionWays.right);
        printf("to the left: %d\n", copy->possibleIntersectionWays.left);
        printf("forward: %d\n", copy->possibleIntersectionWays.forward);
        printf("Direction taken: %d\n\n", copy->currentDirection);
        x++;
        copy = copy->parent;
    } while (copy != nullptr);
}

void IntersectionDetails::printAllNodes() {
    intersectionNode *root = getRoot(currentNode);
    printAllNodesRecursive(root);
}

void IntersectionDetails::printCurrentNode() {
    intersectionNode *copy = currentNode;
    printf("\nPossible directions of chosen node:\n");
    printf("to the right: %d\n", copy->possibleIntersectionWays.right);
    printf("to the left: %d\n", copy->possibleIntersectionWays.left);
    printf("forward: %d\n", copy->possibleIntersectionWays.forward);
    printf("Direction taken: %d\n\n", copy->currentDirection);
    printf("ID %s\n\n", copy->ID.c_str());
}


void IntersectionDetails::printCurrentNode(intersectionNode *node) {
    printf("\nPossible directions of chosen node:\n");
    printf("to the right: %d\n", node->possibleIntersectionWays.right);
    printf("to the left: %d\n", node->possibleIntersectionWays.left);
    printf("forward: %d\n", node->possibleIntersectionWays.forward);
    printf("Direction taken: %d\n\n", node->currentDirection);
    printf("ID %s\n\n", node->ID.c_str());
}




direction IntersectionDetails::getCurrentDirection() {
    return currentNode->currentDirection;
}

string IntersectionDetails::getID() {
    return currentNode->ID;
}

IntersectionWays IntersectionDetails::getPossibleIntersectionWays() {
    return currentNode->possibleIntersectionWays;
}


turnDirection IntersectionDetails::chooseNextDirection() {

    IntersectionWays possibleWays = getIntersectionPossibleWays();

    switch (currentNode->currentDirection) {
        case nothing:
            returnToLastIntersection = false;
            if (possibleWays.right) {
                currentNode->currentDirection = direction::right;
                return turnRight;
            }
            if (possibleWays.forward) {
                currentNode->currentDirection = direction::forward;
                return goStraight;
            }
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                return turnLeft;
            }
            currentNode->currentDirection = direction::backwards;
            returnToLastIntersection = true;
            return turnBackwards;

        case direction::right:
            returnToLastIntersection = false;
            if (possibleWays.forward) {
                currentNode->currentDirection = direction::forward;
                return turnRight;
            }
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                return goStraight;
            }
            currentNode->currentDirection = direction::backwards;
            returnToLastIntersection = true;
            return turnLeft;

        case direction::forward:
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                returnToLastIntersection = false;
                return turnRight;
            }
            currentNode->currentDirection = direction::backwards;
            returnToLastIntersection = true;
            return goStraight;

        case direction::left:
            currentNode->currentDirection = direction::backwards;
            returnToLastIntersection = true;
            return turnRight;
    }
}


IntersectionWays IntersectionDetails::getIntersectionPossibleWays() {
    IntersectionWays intersectionWays = {false, false, false};
    if (currentNode->possibleIntersectionWays.right)
        intersectionWays.right = true;
    if (currentNode->possibleIntersectionWays.left)
        intersectionWays.left = true;
    if (currentNode->possibleIntersectionWays.forward)
        intersectionWays.forward = true;
    return intersectionWays;
}

bool IntersectionDetails::returnToLastIntersectionLogic() {
    if (currentNode->parent!=nullptr) {
        currentNode = currentNode->parent;
        return false;
    }
    return true;
}

bool IntersectionDetails::checkIfNodeIsRoot() {
    if (currentNode->parent!=nullptr) {
        return false;
    }
    return true;
}


intersectionNode *IntersectionDetails::getRoot(intersectionNode *initialNode) {
    intersectionNode *current = initialNode;
    while (current->parent!=nullptr)
        current = current->parent;
    return current;
}

void IntersectionDetails::printAllNodesRecursive(intersectionNode *node) {
    if (node!=nullptr) {
        printCurrentNode(node);
        printAllNodesRecursive(currentNode->right);
        printAllNodesRecursive(currentNode->forward);
        printAllNodesRecursive(currentNode->left);
    }
}



string IntersectionDetails::buildPathFromAToB(string a, string b) {
    string commonAncestor = getLowestCommonAncestor(a, b);
    string pathFromAToB = buildPathFromAToCommonAncestor(a, commonAncestor);
    char turnValueOfA, turnValueOfB;
    getAtoBDifferentDirection(turnValueOfA, turnValueOfB, a, b);
    pathFromAToB += chooseDirectionFromAToBInCriticalPoint(turnValueOfA, turnValueOfB);
    pathFromAToB += buildPathFromCommonAncestorToB(pathFromAToB, commonAncestor);
    return pathFromAToB;
}

string IntersectionDetails::getLowestCommonAncestor(const string &a, const string &b) {
    string commonAncestor;
    for (int i:a) {
        if (a[i] == b[i])
            commonAncestor += a[i];
    }
    return commonAncestor;
}

string IntersectionDetails::buildPathFromAToCommonAncestor(string a, const string& commonAncestor) {
    string pathFromAToCommonAncestor;
    while (true) {
        string copy = a;
        copy.pop_back();
        if (compareStrings(copy, commonAncestor)) {
            break;
        }
        char lastTurn = a[a.length()-1];
        if (lastTurn == 'R')
            pathFromAToCommonAncestor += 'L';
        else if (lastTurn == 'L')
            pathFromAToCommonAncestor += 'R';
        else
            pathFromAToCommonAncestor += lastTurn;
        a.pop_back();
    }
    return pathFromAToCommonAncestor;
}


bool IntersectionDetails::compareStrings(const string& a, const string& b) {
    return !strcmp(a.c_str(), b.c_str());
}


void IntersectionDetails::getAtoBDifferentDirection(char &x, char &y, const string &a, const string &b) {
    for (int i:a) {
        if (a[i] != b[i]) {
            x = a[i];
            y = b[i];
            break;
        }
    }
}

char IntersectionDetails::chooseDirectionFromAToBInCriticalPoint(const char &x, const char &y) {
    switch (x) {
        case 'R':
            if (y == 'L')
                return 'F';
            if (y == 'F')
                return 'R';
        case 'L':
            if (y == 'R')
                return 'F';
            if (y == 'F')
                return 'L';
        case 'F':
            if (y == 'L')
                return 'R';
            if (y == 'R')
                return 'L';
        default:
            return '0';
    }
}

string IntersectionDetails::buildPathFromCommonAncestorToB(string buildPath,  string b) {
    b.erase(0, buildPath.length()+3);
    for (int i:b) {
        buildPath += b[i];
    }
    return buildPath;
}













