#ifndef _FLOOR_H_
#define _FLOOR_H_

#include "Person.h"
#include "Utility.h"
#include <iostream>

class Floor {
private:
    Person people[MAX_PEOPLE_PER_FLOOR]; // 存储当前楼层上的人员数组
    int numPeople; // 当前楼层上的人数
    bool hasUpRequest; // 是否有上行请求
    bool hasDownRequest; // 是否有下行请求

    // 重置楼层的请求状态
    void resetRequests();

public:
    // 添加一个人员到当前楼层，同时指定该人员的请求方向
    void addPerson(Person newPerson, int request);

    // 从当前楼层移除指定索引的人员
    void removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR], const int numPeopleToRemove);

    // 当前楼层的时钟周期处理函数，返回值表示是否有人进出电梯
    int tick(int currentTime);

    // 构造函数
    Floor();

    // 根据索引获取当前楼层的人员对象
    Person getPersonByIndex(int index) const;

    // 获取当前楼层上的人数
    int getNumPeople() const;

    // 设置当前楼层是否有上行请求
    void setHasUpRequest(bool hasRequest);

    // 获取当前楼层是否有上行请求
    bool getHasUpRequest() const;

    // 设置当前楼层是否有下行请求
    void setHasDownRequest(bool hasRequest);

    // 获取当前楼层是否有下行请求
    bool getHasDownRequest() const;

    // 打印当前楼层的选人菜单
    void printFloorPickupMenu(std::ostream& outs) const;

    // 打印当前楼层的第一行美观输出信息
    void prettyPrintFloorLine1(std::ostream& outs) const;

    // 打印当前楼层的第二行美观输出信息
    void prettyPrintFloorLine2(std::ostream& outs) const;
};
#endif
