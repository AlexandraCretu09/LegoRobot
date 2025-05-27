cmake_minimum_required(VERSION 3.29)


project(brick_pi_clion CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER /opt/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /opt/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu-g++)
add_executable(brick_pi_clion main.cpp
        brickpi/BrickPi3.cpp
        brickpi/spi.cpp
        Motor/motor.cpp
        Movement/rotation.cpp
        Movement/wheelsMovement.cpp
        PID/PID.cpp
        PID/PID.h
        Sensors/getSensorData.cpp
        common.cpp
        Resource.cpp
        MonitorGyroscope/MonitorIfStuck.h
        MonitorGyroscope/MonitorIfStuck.cpp
        CheckForIntersection/CheckForIntersection.cpp
        CheckForIntersection/CheckForIntersection.h
        SpecialCases/SpecialCases.cpp
        SpecialCases/SpecialCases.h
        MappingLogic/IntersectionDetails.cpp
        MappingLogic/IntersectionDetails.h
        FileProcessing/FileProcessing.cpp
        FileProcessing/FileProcessing.h

)

target_include_directories(brick_pi_clion PUBLIC brickpi)
target_link_libraries(brick_pi_clion pthread)