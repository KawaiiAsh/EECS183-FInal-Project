#include "Elevator.h"
#include <iostream>

using namespace std;

void Elevator::tick(int currentTime) {

    // 检查当前时间是否可以移动电梯，并且电梯正在服务请求
    if (currentTime % TICKS_PER_ELEVATOR_MOVE == 0 && servicing) {

        // 将当前楼层向目标楼层方向移动1
        if (currentFloor < targetFloor) {
            currentFloor++;
        } else if (currentFloor > targetFloor) {
            currentFloor--;
        }

        // 如果当前楼层已经是目标楼层，则标记服务完成
        if (currentFloor == targetFloor) {
            servicing = false;
        }
    }
}

// 设置目标楼层并标记电梯正在服务
void Elevator::serviceRequest(int floorNum) {
    targetFloor = floorNum;
    servicing = true;
}

// 打印当前状态
void Elevator::print(ostream &outs) {
    outs << currentFloor;
    if (!servicing){
        outs << "w"; // 如果没有服务，则打印"w"
    } else {
        outs << "s" << targetFloor; // 如果正在服务，则打印"s"后跟目标楼层
    }
}

Elevator::Elevator() {
    currentFloor = 0;
    servicing = false;
    targetFloor = 0;
}

// 设置当前楼层
void Elevator::setCurrentFloor(int currentFloorIn) {
    currentFloor = currentFloorIn;
}

// 返回电梯是否正在服务
bool Elevator::isServicing() const {
    return servicing;
}

// 返回当前楼层
int Elevator::getCurrentFloor() const {
    return currentFloor;
}

// 返回目标楼层
int Elevator::getTargetFloor() const {
    return targetFloor;
}
