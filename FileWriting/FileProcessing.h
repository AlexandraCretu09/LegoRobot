//
// Created by alexandra on 5/6/25.
//

#ifndef FILEWRITING_H
#define FILEWRITING_H

#include <string>

#include "../common.h"


class IntersectionDetails;

class FileProcessing {
public:
    FileProcessing();
    void writeToFileNewIntersection(IntersectionDetails result, double distanceTravelled);
    void writeToFileReturningToLastIntersection(IntersectionDetails result);
    void writeToFileFinishedLabyrinth();
    int readFromFileIfManualOrAuto(); //0 - manual, 1 - auto, -1 - error

private:
    std::string writingPipePathString = "/root/LegoRobotOutputFile/backend_sending_node_data";
    std::string readingPipePathString = "/root/LegoRobotOutputFile/frontend_sending_initial_command";
    char * writingPipePath = "/root/LegoRobotOutputFile/frontend_sending_initial_command";
    char * readingPipePath = "/root/LegoRobotOutputFile/frontend_sending_initial_command";

    void initializeFifo(std::string s);
};



#endif //FILEWRITING_H
