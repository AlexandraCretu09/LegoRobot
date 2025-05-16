//
// Created by alexandra on 5/6/25.
//

#ifndef FILEWRITING_H
#define FILEWRITING_H

#include <atomic>
#include <string>
#include <thread>

#include "../common.h"


class IntersectionDetails;

class FileProcessing {
public:
    FileProcessing();
    FileProcessing(std::atomic<bool> &flagForSwitchingBetweenAutoAndManual, std::atomic<bool> &stopFlag);
    void writeToFileNewIntersection(IntersectionDetails result, double distanceTravelled);
    void writeToFileReturningToLastIntersection(IntersectionDetails result);
    void writeToFileFinishedLabyrinth();
    void writeToFileMessage(const std::string& message);
    char readFromFileOneLetterCommand(); //m - manual, a - auto, 0 - error THEN wasd, l - start autonomous maze exploration, x - exit
    void readFromFileIfSwitchFromAutoToManual(); //any character, but only one
    void startMonitoring();
    void stopMonitoring();
    void stopMonitoringAndClosePipe();

private:
    std::atomic<bool> *flagForSwitchingBetweenAutoAndManual;
    std::atomic<bool> *stopFlag;
    bool isMonitoring;
    std::thread monitorThread;

    std::string writingPipePathString = "/root/LegoRobotOutputFile/backend_sending_node_data";
    std::string readingPipePathString = "/root/LegoRobotOutputFile/frontend_sending_command";
    std::string readingPipeForStoppingDuringAuto = "/root/LegoRobotOutputFile/frontend_sending_switching_from_auto_to_manual_command";

    void initializeFifo(std::string s);
    void writeToSwitchingBetweenAutoAndManual();
};



#endif //FILEWRITING_H
