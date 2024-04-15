#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <string>
using namespace std;

// 新文件名
const string NEW_FILENAME = "new.in";
// 加载文件名
//const string LOAD_FILENAME = "save.in";
const string LOAD_FILENAME = "morningPeacefulGame.in";

// 游戏输入文件名
const string GAME_INPUT_FILENAME = "game.in";
// 结果文件名
const string RESULT_FILENAME = "result.csv";

// 电梯容量
const int ELEVATOR_CAPACITY = 10;
// 楼层数量
const int NUM_FLOORS = 10;
// 第一层
const int FIRST_FLOOR = 0;
// 最大愤怒值
const int MAX_ANGER = 10;
// 电梯数量
const int NUM_ELEVATORS = 3;
// 每层最大人数
const int MAX_PEOPLE_PER_FLOOR = 10;
// 最大回合数
const int MAX_TURNS = 100;

// 每过多少回合愤怒值增加一次
const int TICKS_PER_ANGER_INCREASE = 5;
// 每过多少回合电梯移动一次
const int TICKS_PER_ELEVATOR_MOVE = 1;

// 每次爆炸损失的分数
const int POINTS_LOST_PER_EXPLOSION = -10;

// 是否启用AI重写
const bool IS_AI_OVERRIDE = false;

#endif
