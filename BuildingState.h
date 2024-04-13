#ifndef BUILDING_STATE_H
#define BUILDING_STATE_H

#include "Utility.h"

// 人员结构体
struct _Person {
    int angerLevel = 0; // 愤怒等级，默认为0
};

// 楼层结构体
struct _Floor {
    int floorNum = 0; // 楼层编号，默认为0
    int numPeople = 0; // 该楼层的人数
    bool hasUpRequest = false; // 是否有向上请求
    bool hasDownRequest = false; // 是否有向下请求
    _Person people[MAX_PEOPLE_PER_FLOOR]; // 存储在该楼层的人员数组
};

// 电梯结构体
struct _Elevator {
    int elevatorId = 0; // 电梯编号
    int targetFloor = 0; // 目标楼层
    int currentFloor = 0; // 当前楼层
    bool isServicing = false; // 是否正在服务中
};

// 建筑状态结构体
struct BuildingState {
    int turn = 0; // 当前轮次
    _Floor floors[NUM_FLOORS]; // 楼层数组
    _Elevator elevators[NUM_ELEVATORS]; // 电梯数组
};

#endif
