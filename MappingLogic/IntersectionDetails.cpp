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
        case noDirection:
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
        printAllNodesRecursive(node->right);
        printAllNodesRecursive(node->forward);
        printAllNodesRecursive(node->left);
    }
}

void IntersectionDetails::printAllNodesID() {
    intersectionNode *copy = currentNode;
    intersectionNode *root = getRoot(copy);
    printAllNodesIDPrivate(root);
}


void IntersectionDetails::printAllNodesIDPrivate(intersectionNode *node) {
    if (node!=nullptr) {
        printf("Node ID: %s\n", node->ID.c_str());
        printAllNodesIDPrivate(node->right);
        printAllNodesIDPrivate(node->forward);
        printAllNodesIDPrivate(node->left);
    }
}


string IntersectionDetails::buildPathFromAToB(string a, string b) {
    printf("\n\nIn build path from A to B\n");
    string commonAncestor = getLowestCommonAncestor(a, b);
    string pathFromAToB;
    if (commonAncestor == a) {
        pathFromAToB = buildPathFromCommonAncestorToB(pathFromAToB, commonAncestor, b);
    } else if (commonAncestor == b) {
        pathFromAToB = buildPathFromAToCommonAncestor(a, commonAncestor);
    }else {
        pathFromAToB = buildPathFromAToCommonAncestor(a, commonAncestor);
        char turnValueOfA, turnValueOfB;
        getAtoBDifferentDirection(turnValueOfA, turnValueOfB, a, b);
        pathFromAToB += chooseDirectionFromAToBInCriticalPoint(turnValueOfA, turnValueOfB);
        // printf("result of choose critical point direction: %s\n", pathFromAToB.c_str());
        pathFromAToB = buildPathFromCommonAncestorToB(pathFromAToB, commonAncestor, b);
    }
    return pathFromAToB;
}

string IntersectionDetails::getLowestCommonAncestor(string a, string b) {
    // printf("In get lowest common ancestor\n");
    string commonAncestor;
    for (int i=0; i< a.length(); i++) {
        if (a[i] == b[i])
            commonAncestor += a[i];
    }
    // printf("Ancestor: %s\n", commonAncestor.c_str());
    return commonAncestor;
}

string IntersectionDetails::buildPathFromAToCommonAncestor(string a,  string commonAncestor) {

    // printf("In build path from A to common ancestor\n");
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
    // printf("path: %s\n", pathFromAToCommonAncestor.c_str());
    return pathFromAToCommonAncestor;
}


bool IntersectionDetails::compareStrings( string a,  string b) {
    // printf("In comparing strings\n");
    // printf("a: %s, b: %s\n\n", a.c_str(), b.c_str());
    return !strcmp(a.c_str(), b.c_str());
}


void IntersectionDetails::getAtoBDifferentDirection(char &x, char &y, const string a, const string b) {
    // printf("In get A to B Different Direction\n");
    for (int i = 0;i<a.length();i++) {
        if (a[i] != b[i]) {
            x = a[i];
            y = b[i];
            break;
        }
    }
}
//toDO: if A is a part of B
char IntersectionDetails::chooseDirectionFromAToBInCriticalPoint(char x, char y) {
    // printf("in choose direction from A to B in critical point\n");
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

string IntersectionDetails::buildPathFromCommonAncestorToB(string buildPath, string commonAncestor,  string b) {
    printf("In build path from common ancestor to B\n");
    printf("Built path: %s\n", buildPath.c_str());
    printf("Full B: %s\n", b.c_str());
    b.erase(0, commonAncestor.length()+1);
    printf("Full B: %s\n", b.c_str());
    for (int i=0; i<b.length(); i++) {
        printf("B: %c\n", b[i]);
        buildPath += b[i];
    }
    printf("path: %s\n", buildPath.c_str());
    return buildPath;
}












