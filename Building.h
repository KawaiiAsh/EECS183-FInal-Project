#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "Elevator.h"
#include "Floor.h"
#include "Person.h"
#include "Utility.h"
#include "Move.h"
#include "BuildingState.h"
#include <string>
#include <iostream>
#include <fstream>

class Building {
private:
    Elevator elevators[NUM_ELEVATORS]; // 电梯数组
    Floor floors[NUM_FLOORS]; // 楼层数组
    int time; // 当前时间

    // 更新电梯状态
    void update(Move move);

public:
    // 生成新乘客
    void spawnPerson(Person newPerson);
    // 时间流逝
    int tick(Move move);
    // 构造函数
    Building();
    // 设置电梯信息
    void setElevator(string elevatorInfo, int elevatorId);
    // 获取当前时间
    int getTime() const;
    // 设置当前时间
    void setTime(int timeIn);
    // 打印建筑信息
    void prettyPrintBuilding(ostream& outs) const;
    // 根据电梯ID获取电梯对象
    Elevator getElevatorById(int elevatorId) const;
    // 根据楼层号获取楼层对象
    Floor getFloorByFloorNum(int floorNum) const;
    // 获取建筑状态
    BuildingState getBuildingState() const;
};

#endif
