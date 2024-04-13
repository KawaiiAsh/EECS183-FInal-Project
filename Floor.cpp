#include "Floor.h"
#include <algorithm>

using namespace std;

// Floor 类的成员函数 tick
int Floor::tick(int currentTime) {
    int count = 0; // 计数器
    int idx[MAX_PEOPLE_PER_FLOOR]; // 用于存储需要移除的人的索引数组

    // 遍历每个人，检查是否需要移除
    for (int i = 0; i < numPeople; i++) {
        if (people[i].tick(currentTime)) { // 如果这个人需要移除
            idx[count] = i; // 将需要移除的人的索引记录到 idx 数组中
            count++; // 计数器加一
        }
    }
    removePeople(idx, count); // 调用 removePeople 函数移除人
    return count; // 返回移除的人数
}

// Floor 类的成员函数 addPerson
void Floor::addPerson(Person newPerson, int request) {
    if (numPeople < MAX_PEOPLE_PER_FLOOR) { // 如果人数未达到上限
        people[numPeople] = newPerson; // 将新的人添加到人数组中
        numPeople++; // 人数加一
    }

    // 如果有请求
    if (request != 0) {
        // 根据请求类型设置上行或下行请求标志
        if (request > 0) {
            hasUpRequest = true;
        } else {
            hasDownRequest = true;
        }
    }
}

// Floor 类的成员函数 removePeople
void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
                         int numPeopleToRemove) {
    int sortedIndices[MAX_PEOPLE_PER_FLOOR]; // 存储需要移除的人的索引的数组
    copy(indicesToRemove, indicesToRemove + numPeopleToRemove, sortedIndices); // 复制需要移除的人的索引到 sortedIndices 数组
    sort(sortedIndices, sortedIndices + numPeopleToRemove); // 对需要移除的人的索引进行排序

    // 从后往前遍历需要移除的人的索引
    for (int i = numPeopleToRemove - 1; i >= 0; i--) {
        int indexToRemove = sortedIndices[i]; // 获取需要移除的人的索引
        // 将该索引后面的人向前移动一个位置
        for (int j = indexToRemove; j < numPeople - 1; j++) {
            people[j] = people[j + 1];
        }
        numPeople--; // 人数减一
    }
    resetRequests(); // 重置请求标志
}

// Floor 类的成员函数 resetRequests
void Floor::resetRequests() {
    hasUpRequest = false; // 重置上行请求标志
    hasDownRequest = false; // 重置下行请求标志

    // 遍历每个人，检查其目标楼层以重设请求标志
    for (int i = 0; i < numPeople; i++) {
        int currentFloor = people[i].getCurrentFloor(); // 当前楼层
        int targetFloor = people[i].getTargetFloor(); // 目标楼层

        if (currentFloor < targetFloor) { // 如果当前楼层小于目标楼层
            hasUpRequest = true; // 设置上行请求标志为真
        }
        else if (currentFloor > targetFloor) { // 如果当前楼层大于目标楼层
            hasDownRequest = true; // 设置下行请求标志为真
        }
    }
}

// Floor 类的构造函数
Floor::Floor() {
    hasDownRequest = false; // 初始化下行请求标志为假
    hasUpRequest = false; // 初始化上行请求标志为假
    numPeople = 0; // 初始化人数为0
}

// Floor 类的成员函数 prettyPrintFloorLine1
void Floor::prettyPrintFloorLine1(ostream &outs) const {
    string up = "U"; // 上行标志
    outs << (hasUpRequest ? up : " ") << " "; // 输出上行标志
    // 输出每个人的愤怒级别以及间隔
    for (int i = 0; i < numPeople; ++i) {
        outs << people[i].getAngerLevel();
        outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
    }
    outs << endl; // 换行
}

// Floor 类的成员函数 prettyPrintFloorLine2
void Floor::prettyPrintFloorLine2(ostream &outs) const {
    string down = "D"; // 下行标志
    outs << (hasDownRequest ? down : " ") << " "; // 输出下行标志
    // 输出每个人的状态以及间隔
    for (int i = 0; i < numPeople; ++i) {
        outs << "o   ";
    }
    outs << endl; // 换行
}

// Floor 类的成员函数 printFloorPickupMenu
void Floor::printFloorPickupMenu(ostream &outs) const {
    cout << endl; // 输出换行
    outs << "Select People to Load by Index" << endl; // 输出加载人的提示信息
    outs << "All people must be going in same direction!"; // 提示所有人必须朝着同一个方向
    outs << endl << "              ";
    // 输出加载人的图示
    for (int i = 0; i < numPeople; ++i) {
        outs << " O   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "-|-  ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << " |   ";
    }
    outs << endl << "              ";
    for (int i = 0; i < numPeople; ++i) {
        outs << "/ \\  ";
    }
    outs << endl << "INDEX:        ";
    // 输出人的索引
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << i << "   ";
    }
    outs << endl << "ANGER:        ";
    // 输出人的愤怒级别
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getAngerLevel() << "   ";
    }
    outs << endl << "TARGET FLOOR: ";
    // 输出人的目标楼层
    for (int i = 0; i < numPeople; ++i) {
        outs << " " << people[i].getTargetFloor() << "   ";
    }
}

// Floor 类的成员函数 setHasUpRequest
void Floor::setHasUpRequest(bool hasRequest) {
    hasUpRequest = hasRequest; // 设置上行请求标志
}

// Floor 类的成员函数 getHasUpRequest
bool Floor::getHasUpRequest() const {
    return hasUpRequest; // 返回上行请求标志
}

// Floor 类的成员函数 setHasDownRequest
void Floor::setHasDownRequest(bool hasRequest) {
    hasDownRequest = hasRequest; // 设置下行请求标志
}

// Floor 类的成员函数 getHasDownRequest
bool Floor::getHasDownRequest() const {
    return hasDownRequest; // 返回下行请求标志
}

// Floor 类的成员函数 getNumPeople
int Floor::getNumPeople() const {
    return numPeople; // 返回人数
}

// Floor 类的成员函数 getPersonByIndex
Person Floor::getPersonByIndex(int index) const {
    return people[index]; // 返回指定索引处的人
}
