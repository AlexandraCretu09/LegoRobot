//
// Created by alexa on 2/25/25.
//
#include <iostream>
#include <string.h>
#include "../common.h"
#include "IntersectionDetails.h"
using namespace std;

IntersectionDetails::IntersectionDetails(int gyroscopeReading) {
    intersectionNode *root = new intersectionNode();
    root->initialGyroscopeReading = gyroscopeReading;
    root->forward = nullptr;
    root->left = nullptr;
    root->right = nullptr;
    root->parent = nullptr;
    root->currentDirection = direction::forward;
    root->possibleIntersectionWays = {false, false, true};
    root->visitedIntersectionWays = { false, false, true};
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
            break;
        case direction::right:
            currentNode->right = newNode;
            break;
        case direction::left:
            currentNode->left = newNode;
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

void IntersectionDetails::printAllNodes() {
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

void IntersectionDetails::printCurrentNode() {
    intersectionNode *copy = currentNode;
    printf("\nPossible directions of chosen node:\n");
    printf("to the right: %d\n", copy->possibleIntersectionWays.right);
    printf("to the left: %d\n", copy->possibleIntersectionWays.left);
    printf("forward: %d\n", copy->possibleIntersectionWays.forward);
    printf("Direction taken: %d\n\n", copy->currentDirection);
}




direction IntersectionDetails::getCurrentDirection() {
    return currentNode->currentDirection;
}

turnDirection IntersectionDetails::chooseNextDirection() {

    IntersectionWays possibleWays = getIntersectionPossibleWays();

    switch (currentNode->currentDirection) {
        case nothing:
            printf("in case nothing, ");
            if (possibleWays.right) {
                currentNode->currentDirection = direction::right;
                return turnRight;
            }
            if (possibleWays.forward) {
                printf("forward\n");
                currentNode->currentDirection = direction::forward;
                return goStraight;
            }
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                return turnLeft;
            }
            currentNode->currentDirection = direction::backwards;
            return turnBackwards;

        case direction::right:
            if (possibleWays.forward) {
                currentNode->currentDirection = direction::forward;
                return turnRight;
            }
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                return goStraight;
            }
            currentNode->currentDirection = direction::backwards;
            return turnLeft;

        case direction::forward:
            if (possibleWays.left) {
                currentNode->currentDirection = direction::left;
                return turnRight;
            }
            currentNode->currentDirection = direction::backwards;
            return goStraight;

        case direction::left:
            currentNode->currentDirection = direction::backwards;
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
IntersectionWays IntersectionDetails::getIntersectionVisitedWays() {
    IntersectionWays intersectionWays;
    if (currentNode->visitedIntersectionWays.right)
        intersectionWays.right = true;
    if (currentNode->visitedIntersectionWays.left)
        intersectionWays.left = true;
    if (currentNode->visitedIntersectionWays.forward)
        intersectionWays.forward = true;
    return intersectionWays;
}







