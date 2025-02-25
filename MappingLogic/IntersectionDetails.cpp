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
    currentNode = root;
}

void IntersectionDetails::addNewIntersection() {
    intersectionNode *newNode = new intersectionNode();
    newNode->parent = currentNode;
    newNode->forward = nullptr;
    newNode->left = nullptr;
    newNode->right = nullptr;

    if (currentNode->currentDirection == direction::forward)
        currentNode->forward = newNode;
    if (currentNode->currentDirection == direction::right)
        currentNode->right = newNode;
    if (currentNode->currentDirection == direction::left)
        currentNode->left = newNode;

    currentNode = newNode;
}

void IntersectionDetails::printAllNodes() {
    intersectionNode *copy = currentNode;
    int x = 1;
    do {
        printf("\nDirections of node %d\n", x);
        printf("to the right:%d\n", copy->right);
        printf("to the left:%d\n", copy->left);
        printf("forward:%d\n", copy->forward);
        copy = copy->parent;
    } while (copy != nullptr);
}




