cmake_minimum_required(VERSION 3.29)
project(brick_pi_clion CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(brick_pi_clion main.cpp
        brickpi/BrickPi3.cpp
        brickpi/spi.cpp
        Motor/motor.cpp
        Movement/rotation.cpp
        Movement/wheelsMovement.cpp
        PID/PID.cpp
        Sensors/getSensorData.cpp
        common.cpp
        Resource.cpp
        MonitorGyroscope/GyroMonitor.h
        MonitorGyroscope/GyroMonitor.cpp

)

target_include_directories(brick_pi_clion PUBLIC brickpi)
target_link_libraries(brick_pi_clion pthread)