cmake_minimum_required(VERSION 3.27)
project(starter_files)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(starter_files
        AI.cpp
        AI.h
        Building.cpp
        Building.h
        BuildingState.h
        Elevator.cpp
        Elevator.h
        Floor.cpp
        Floor.h
        Game.cpp
        Game.h
        main.cpp
        Move.cpp
        Move.h
        Person.cpp
        Person.h
        SatisfactionIndex.cpp
        SatisfactionIndex.h
        test.cpp
        Utility.h)
