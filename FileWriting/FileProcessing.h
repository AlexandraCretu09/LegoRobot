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
    void writeToFileReturningToLastIntersection();
    void readFromFile();

private:
    std::string filePathString = "/root/LegoRobotOutputFile/outputFile.txt";
    char * filePath = "/root/LegoRobotOutputFile/outputFile.txt";

    void initializeFifo();
};



#endif //FILEWRITING_H
