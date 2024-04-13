#ifndef _AI_H_
#define _AI_H_

#include <iostream>
#include "Utility.h"
#include "Move.h"
#include "BuildingState.h"
#include "Person.h"

/*
* 要求：buildingState 是一个有效的状态，表示当前的建筑物状态
* 修改：无
* 效果：返回一个字符串，表示 AI 根据建筑物的当前状态做出的决策，
*       它从 buildingState 中读取当前状态。
*       字符串应该与人类玩家的移动输入格式完全相同。
*/
string getAIMoveString(const BuildingState &buildingState);

/*
* 要求：buildingState 是一个有效的状态，表示当前的建筑物状态
*       move 是由 getAIMoveString() 生成的拾取移动
*       floorToPickup 表示拾取移动发生的楼层
* 修改：无
* 效果：返回一个字符串，表示应该拾取哪些人的索引。
*       字符串应该与人类玩家的拾取列表输入格式完全相同。
*/
string getAIPickupList(const Move &move, const BuildingState &buildingState,
                       const Floor &floorToPickup);

#endif
