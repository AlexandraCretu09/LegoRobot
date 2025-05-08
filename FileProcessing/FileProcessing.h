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
    void writeToFileMessage(const std::string& message);
    char readFromFileOneLetterCommand(); //m - manual, a - auto, 0 - error THEN wasd, l - start autonomous maze exploration, x - exit

private:
    std::string writingPipePathString = "/root/LegoRobotOutputFile/backend_sending_node_data";
    std::string readingPipePathString = "/root/LegoRobotOutputFile/frontend_sending_command";
    // char writingPipePath[] = "/root/LegoRobotOutputFile/frontend_sending_initial_command";
    // char readingPipePath[] = "/root/LegoRobotOutputFile/frontend_sending_initial_command";

    void initializeFifo(std::string s);
};



#endif //FILEWRITING_H
