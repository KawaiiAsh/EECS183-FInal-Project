#include "Person.h"
#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;

Person::Person(string inputString) : Person() {
    stringstream ss(inputString);
    char junk;
    ss >> turn;
    ss >> junk; // 丢弃字符
    ss >> currentFloor;
    ss >> junk; // 丢弃字符
    ss >> targetFloor;
    ss >> junk; // 丢弃字符
    ss >> angerLevel;
}

bool Person::tick(int currentTime) {

    // 每 TICKS_PER_ANGER_INCREASE 个时间单位，此函数将人的愤怒等级增加 1。
    if (currentTime % TICKS_PER_ANGER_INCREASE == 0) {
        angerLevel++;
    }

    // 如果增加后，人的愤怒等级达到了 MAX_ANGER，则函数返回 true。
    if (angerLevel >= MAX_ANGER) {
        return true;
    }

        // 如果人的愤怒等级小于 MAX_ANGER，则函数返回 false。
    else {
        return false;
    }
}

// 打印人的信息字符串，不包括转弯信息
void Person::print(ostream &outs) {
    outs << "f" << currentFloor << "t" << targetFloor << "a" << angerLevel << endl;
}

//////////////////////////////////////////////////////
////// 以下代码不得修改 ///////////////////////////////
//////////////////////////////////////////////////////

Person::Person() {
    turn = 0;
    currentFloor = 0;
    targetFloor = 0;
    angerLevel = 0;
}

int Person::getTurn() const {
    return turn;
}

int Person::getCurrentFloor() const {
    return currentFloor;
}

int Person::getTargetFloor() const {
    return targetFloor;
}

int Person::getAngerLevel() const {
    return angerLevel;
}

ostream &operator<<(ostream &outs, Person p) {
    p.print(outs);
    return outs;
}
