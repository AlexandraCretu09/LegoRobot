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

    // if (currentNode->currentDirection == direction::forward)
    //     currentNode->forward = newNode;
    // if (currentNode->currentDirection == direction::right)
    //     currentNode->right = newNode;
    // if (currentNode->currentDirection == direction::left)
    //     currentNode->left = newNode;

    currentNode = newNode;
}

void IntersectionDetails::printAllNodes() {
    intersectionNode *copy = currentNode;
    int x = 1;
    do {
        printf("\nChild nodes of node %d\n", x);
        printf("to the right: %d\n", copy->right);
        printf("to the left: %d\n", copy->left);
        printf("forward: %d\n", copy->forward);
        printf("\nPossible directions of node%d\n", x);
        printf("to the right: %d\n", copy->possibleIntersectionWays.right);
        printf("to the left: %d\n", copy->possibleIntersectionWays.left);
        printf("forward: %d\n", copy->possibleIntersectionWays.forward);
        printf("Direction taken: %d\n\n", copy->currentDirection);
        x++;
        copy = copy->parent;
    } while (copy != nullptr);
}

// IntersectionWays IntersectionDetails::getIntersectionWays() {
//     IntersectionWays intersectionWays;
//      if (currentNode->)
// }





