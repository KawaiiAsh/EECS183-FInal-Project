#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#include "Utility.h" // 包含Utility.h头文件
#include "Person.h" // 包含Person.h头文件
#include <string> // 包含string头文件

class Elevator {
private:
    bool servicing; // 是否处于服务状态
    int currentFloor; // 当前楼层
    int targetFloor; // 目标楼层

public:
    void tick(int currentTime); // 电梯运行一次的操作
    void serviceRequest(int floorNum); // 处理服务请求
    void print(ostream &outs); // 打印电梯状态
    Elevator(); // 构造函数
    void setCurrentFloor(int currentFloorIn); // 设置当前楼层
    bool isServicing() const; // 是否正在服务中
    int getCurrentFloor() const; // 获取当前楼层
    int getTargetFloor() const; // 获取目标楼层
};

#endif
