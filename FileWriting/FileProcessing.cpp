//
// Created by alexandra on 5/6/25.
//

#include "FileProcessing.h"

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

#include "../MappingLogic/IntersectionDetails.h"

using namespace std;

FileProcessing::FileProcessing() {
    initializeFifo();
}


void FileProcessing::writeToFileNewIntersection(IntersectionDetails result, double distanceTravelled) {
    int fd;

    string distance = to_string(distanceTravelled);
    string ID = result.getID();
    string currentDirection = to_string(result.getCurrentDirection());

    IntersectionWays possibleWays = result.getPossibleIntersectionWays();

    string right = possibleWays.right ? "1" : "0";
    string left = possibleWays.left ? "1" : "0";
    string forward = possibleWays.forward ? "1" : "0";

    string output = "{"
                    "\"node_id\": \"" + ID + "\", "
                    "\"distance\": \"" + distance + "\", "
                    "\"possible_ways\": {"
                        "\"right\": " + right + ", "
                        "\"left\": " + left + ", "
                        "\"forward\": " + forward +
                    "}, "
                    "\"current_direction\": \"" + currentDirection + "\""
                    "}\n";



    fd = open(filePath, O_WRONLY);
    write(fd, output.c_str(), output.size());
    close(fd);
}

void FileProcessing::initializeFifo() {
    if (access(filePathString.c_str(), F_OK) == -1) {
        if (mkfifo(filePathString.c_str(), 0666) == -1) {
            perror("Error creating FIFO");
        }
    }
}
