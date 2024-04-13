#ifndef _SATISFACTION_INDEX_H_
#define _SATISFACTION_INDEX_H_

#include <iostream>
#include <fstream>

#include "Utility.h"
#include "Building.h"
#include "Move.h"

class SatisfactionIndex {
private:

    int sumIndex;           // 总指数
    int sumUpRequest;       // 向上请求总数
    int sumDownRequest;     // 向下请求总数
    int sumExploded;        // 爆炸总数
    int sumIdle;            // 空闲总数
    int timeReached;        // 达到时间

public:

    SatisfactionIndex();    // 构造函数

    int getSatisfaction() const;    // 获取满意度

    void updateSumDirectionRequest(const Move&, const Building&);    // 更新方向请求总数

    void updateSumExploded(const int);    // 更新爆炸总数

    void updateSumIdle(const Building&);    // 更新空闲总数

    void updateTimeReached(const Building&);    // 更新达到时间

    void readRawSatisfaction(istream& ins);    // 读取原始满意度

    void printRawSatisfaction(ostream& outs) const;    // 打印原始满意度

    void printSatisfaction(ostream& outs, const bool isCSV = false) const;    // 打印满意度

    void save() const;    // 保存数据
};

ostream& operator<<(ostream& outs, const SatisfactionIndex& index);    // 输出流运算符重载
istream& operator>>(istream& ins, SatisfactionIndex& index);    // 输入流运算符重载

#endif
