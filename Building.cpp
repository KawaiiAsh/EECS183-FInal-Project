#include "Building.h"

using namespace std;

// 建筑类的方法实现

// 向指定楼层添加一个新的人物
void Building::spawnPerson(Person newPerson) {
    // 将新人物添加到目标楼层，并计算需要移动的楼层数
    floors[newPerson.getCurrentFloor()].addPerson(
            newPerson,
            newPerson.getTargetFloor() - newPerson.getCurrentFloor());
}

// 更新建筑状态
void Building::update(Move move) {
    int targetFloor, elevatorId, numPeopleToPickup = 0;
    int pickupsArray[10];

    targetFloor = move.getTargetFloor();
    elevatorId = move.getElevatorId();
    numPeopleToPickup = move.getNumPeopleToPickup();

    // 如果是空转、存档或退出动作，不进行更新
    if (move.isPassMove() || move.isSaveMove() || move.isQuitMove()) {
        return;
    } else {
        // 否则，处理电梯服务请求
        elevators[elevatorId].serviceRequest(targetFloor);
        if (move.isPickupMove()) {
            // 如果是乘客上电梯动作，则从对应楼层移除乘客
            move.copyListOfPeopleToPickup(pickupsArray);
            floors[elevators[elevatorId]
                    .getCurrentFloor()]
                    .removePeople(pickupsArray, numPeopleToPickup);
        }
    }
}

// 更新时间和所有电梯、楼层的状态
int Building::tick(Move move) {
    time++; // 时间增加

    update(move); // 更新建筑状态

    int numExplodedPeople = 0;
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        elevators[i].tick(time); // 更新每个电梯的状态
    }
    for (int i = 0; i < NUM_FLOORS; i++) {
        numExplodedPeople += floors[i].tick(time); // 更新每个楼层的状态，并统计爆炸的乘客数
    }
    return numExplodedPeople;
}

// 建筑类的构造函数
Building::Building() {
    time = 0; // 初始化时间
}

// 设置指定id的电梯
void Building::setElevator(string elevatorInfo, int elevatorId) {
    int i = 0;
    string currPosString = "";
    string destPosString = "";

    // 解析电梯信息，包括当前位置和目标位置
    while (elevatorInfo[i] != 's' && elevatorInfo[i] != 'w') {
        currPosString += elevatorInfo[i];
        i++;
    }
    elevators[elevatorId] = Elevator();

    elevators[elevatorId].setCurrentFloor(stoi(currPosString)); // 设置当前位置

    if (elevatorInfo[i] == 's') {
        i++;
        while (i < (int)elevatorInfo.size()) {
            destPosString += elevatorInfo[i];
            i++;
        }
        elevators[elevatorId].serviceRequest(stoi(destPosString)); // 设置目标位置
    }
}

// 获取当前时间
int Building::getTime() const {
    return time;
}

// 设置时间
void Building::setTime(int timeIn) {
    time = timeIn;
}

// 打印建筑状态
void Building::prettyPrintBuilding(ostream& outs) const {
    // 从顶层向底层打印每一层的状态
    for (int i = NUM_FLOORS - 1; i >= 0; --i) {
        outs << "   ";
        for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {
            outs << '-';
        }
        outs << endl << "   ";

        // 打印每个电梯当前位置和服务状态
        for (int j = 0; j <  NUM_ELEVATORS; ++j) {
            char floorIndicatorIcon = ' ';

            if (elevators[j].getCurrentFloor() == i &&
                elevators[j].isServicing()) {

                char floorServiceIndicator = ('0' + elevators[j].getTargetFloor());
                floorIndicatorIcon = floorServiceIndicator;
            }

            outs << "|" << floorIndicatorIcon;
        }
        outs << "|";

        floors[i].prettyPrintFloorLine1(outs);
        outs << i << "  ";
        for (int j = 0; j <  NUM_ELEVATORS; ++j) {

            outs << "|" << (elevators[j].getCurrentFloor() == i ? 'E' : ' ');
        }
        outs << "|";

        floors[i].prettyPrintFloorLine2(outs);
    }
    outs << "   ";
    for (int j = 0; j < 2 * NUM_ELEVATORS + 1; ++j) {
        outs << '-';
    }
    outs << endl << "   ";
    for (int j = 0; j < NUM_ELEVATORS; ++j) {
        outs << " " << j;
    }
    outs << endl;
}

// 根据电梯id获取电梯状态
Elevator Building::getElevatorById(int elevatorId) const {
    return elevators[elevatorId];
}

// 根据楼层数获取楼层状态
Floor Building::getFloorByFloorNum(int floorNum) const {
    return floors[floorNum];
}

// 获取建筑整体状态
BuildingState Building::getBuildingState() const {
    BuildingState buildingState;
    buildingState.turn = time; // 记录当前时间

    // 遍历每一层楼
    for (int i = 0; i < NUM_FLOORS; ++i) {
        buildingState.floors[i].floorNum = i; // 记录楼层编号
        buildingState.floors[i].hasDownRequest = floors[i].getHasDownRequest(); // 记录楼层是否有向下请求
        buildingState.floors[i].hasUpRequest = floors[i].getHasUpRequest(); // 记录楼层是否有向上请求

        int & np = buildingState.floors[i].numPeople;

        // 遍历每个楼层的乘客
        for (int j = 0; j < floors[i].getNumPeople(); ++j) {
            Person p = floors[i].getPersonByIndex(j);
            buildingState.floors[i].people[np++].angerLevel = p.getAngerLevel(); // 记录乘客的愤怒程度
        }
    }

    // 遍历每个电梯
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        Elevator e = elevators[i];

        // 记录电梯的状态
        buildingState.elevators[i].elevatorId = i;
        buildingState.elevators[i].targetFloor = e.getTargetFloor();
        buildingState.elevators[i].currentFloor = e.getCurrentFloor();
        buildingState.elevators[i].isServicing = e.isServicing();
    }

    return buildingState;
}
