//
// Created by alexandra on 5/6/25.
//

#include "FileProcessing.h"

#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
// #inlcude <uwebsockets/App.h>

#include "../MappingLogic/IntersectionDetails.h"

using namespace std;

FileProcessing::FileProcessing() : flagForSwitchingBetweenAutoAndManual(nullptr), stopFlag(nullptr), isMonitoring(false) {
    initializeFifo(writingPipePathString);
    initializeFifo(readingPipePathString);
}

FileProcessing::FileProcessing(atomic<bool> &flagForSwitchingBetweenAutoAndManual, atomic<bool> &stopFlag)
    : flagForSwitchingBetweenAutoAndManual(&flagForSwitchingBetweenAutoAndManual), stopFlag(&stopFlag),
    isMonitoring(false) {
    initializeFifo(readingPipeForStoppingDuringAuto);
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



    fd = open(writingPipePathString.c_str(), O_WRONLY);
    write(fd, output.c_str(), output.size());
    close(fd);
}

void FileProcessing::writeToFileReturningToLastIntersection(IntersectionDetails result) {

    string ID = result.getID();
    string isReturning = "true";
    string currentDirection = to_string(result.getCurrentDirection());

    string output="{"
                    "\"return\": \"" + isReturning + "\", "
                    "\"node_id\": \"" + ID + "\", "
                    "\"current_direction\": \"" + currentDirection + "\""
                    "}\n";
    int fd = open(writingPipePathString.c_str(), O_WRONLY);
    write(fd, output.c_str(), output.size());
    close(fd);
}

void FileProcessing::writeToFileFinishedLabyrinth() {
    string finishedLabyrinth = "true";
    string output = "{"
                    "\"finishedLabyrinth\": \"" + finishedLabyrinth + "\""
                    "}\n";
    int fd = open(writingPipePathString.c_str(), O_WRONLY);
    write(fd, output.c_str(), output.size());
    close(fd);
}

void FileProcessing::writeToFileMessage(const string &message) {
    printf("Writing feedback message\n");
    int fd = open(writingPipePathString.c_str(), O_WRONLY);
    write(fd, message.c_str(), message.size());
    close(fd);
}


char FileProcessing::readFromFileOneLetterCommand() {

    int fifo = open(readingPipePathString.c_str(), O_RDONLY);
    if (fifo == -1) {
        perror("Error opening FIFO for reading\n");
        return '0';
    }
    char command;
    ssize_t bytesRead = read(fifo, &command, sizeof(command));

    if (bytesRead == -1) {
        perror("Error reading from FIFO\n");
        close(fifo);
        return '0';
    }

    if (bytesRead > 0) {
        printf("Received letter: %c\n", command);
        close(fifo);
        return command;

    }

    perror("No data was read");
    close(fifo);
    return '0';
}

void FileProcessing::readFromFileIfSwitchFromAutoToManual() {
    int fifo = open(readingPipeForStoppingDuringAuto.c_str(), O_RDONLY);
    if (fifo == -1) {
        perror("Error opening FIFO for reading\n");
    }
    char command;
    ssize_t bytesRead = read(fifo, &command, sizeof(command));

    if (bytesRead == -1) {
        perror("Error reading from FIFO\n");
        close(fifo);
    }

    if (bytesRead > 0) {
        // printf("Received letter: %c\n", command);
        stopFlag->store(true);
        close(fifo);
        if (command == 'x') {
            // printf("Shutdown command received.\n");
            return;
        }
        flagForSwitchingBetweenAutoAndManual->store(true);
        // stopFlag->store(true);
    }
    perror("No data was read");
    close(fifo);
}

void FileProcessing::writeToSwitchingBetweenAutoAndManual() {
    int fd = open(readingPipeForStoppingDuringAuto.c_str(), O_WRONLY);
    string a = "x";
    write(fd, a.c_str(), a.size());
    close(fd);
}


void FileProcessing::startMonitoring() {
    isMonitoring = true;
    // printf("Started monitoring the file\n");
    monitorThread = thread(&FileProcessing::readFromFileIfSwitchFromAutoToManual, this);
}

void FileProcessing::stopMonitoring() {
    if (isMonitoring) {
        isMonitoring = false;
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
    }
}

void FileProcessing::stopMonitoringAndClosePipe() {
    writeToSwitchingBetweenAutoAndManual();
    if (isMonitoring) {
        isMonitoring = false;
        if (monitorThread.joinable()) {
            monitorThread.join();
        }
    }
}

void FileProcessing::initializeFifo(string path) {
    if (access(path.c_str(), F_OK) == -1) {
        if (mkfifo(path.c_str(), 0666) == -1) {
            perror("Error creating FIFO");
        }
    }
}
