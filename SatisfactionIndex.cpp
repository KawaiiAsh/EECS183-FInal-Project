#include "SatisfactionIndex.h"
using namespace std;

SatisfactionIndex::SatisfactionIndex() {
    sumIndex = 0;
    sumUpRequest = 0;
    sumDownRequest = 0;
    sumExploded = 0;
    sumIdle = 0;
    timeReached = 0;
}

int SatisfactionIndex::getSatisfaction() const {
    return sumIndex;
}

void SatisfactionIndex::updateSumDirectionRequest(const Move& move, const Building& building) {
    // 更新满意度指数
    sumIndex += move.getTotalSatisfaction();

    int currentElevatorFloor = building.getElevatorById(move.getElevatorId()).getCurrentFloor();
    int targetFloor = move.getTargetFloor();

    // 根据当前电梯楼层和目标楼层更新上下请求总数
    if (currentElevatorFloor < targetFloor) {
        sumUpRequest++; // 更新上行请求总数
    } else {
        sumDownRequest++; // 更新下行请求总数
    }
}

void SatisfactionIndex::updateSumExploded(const int num) {
    // 更新爆炸次数和相应的满意度指数
    sumExploded += num;
    sumIndex += (num * POINTS_LOST_PER_EXPLOSION);
}

void SatisfactionIndex::updateSumIdle(const Building& building) {
    // 更新空闲电梯总数
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        if (!building.getElevatorById(i).isServicing()) {
            ++sumIdle;
        }
    }
}

void SatisfactionIndex::updateTimeReached(const Building& b) {
    // 更新达到目的地的时间
    timeReached = b.getTime();
    return;
}

void SatisfactionIndex::readRawSatisfaction(istream& ins) {
    // 从输入流中读取原始满意度数据
    ins >> sumIndex >> sumUpRequest >> sumDownRequest >> sumExploded >> sumIdle;
}

void SatisfactionIndex::printRawSatisfaction(ostream& outs) const {
    // 打印原始满意度数据到输出流
    outs << sumIndex << endl;
    outs << sumUpRequest << endl;
    outs << sumDownRequest << endl;
    outs << sumExploded << endl;
    outs << sumIdle << endl;
}

void SatisfactionIndex::printSatisfaction(ostream& outs, const bool isCSV) const {
    // 根据指定格式打印满意度数据到输出流
    if (!isCSV) {
        // 非CSV格式
        outs << "sumIndex:      \t" << sumIndex << endl;
        outs << "sumUpRequest:  \t" << sumUpRequest << endl;
        outs << "sumDownRequest:\t" << sumDownRequest << endl;
        outs << "sumExploded:   \t" << sumExploded << endl;
        outs << "sumIdle:       \t" << sumIdle << endl;
        outs << "timeReached:   \t" << timeReached << endl;
    } else {
        // CSV格式
        outs << "test,sumIndex,sumUpRequest,sumDownRequest,sumExploded,sumIdle,timeReached" << endl;
        outs << "student,";
        outs << sumIndex << ',';
        outs << sumUpRequest << ',';
        outs << sumDownRequest << ',';
        outs << sumExploded << ',';
        outs << sumIdle << ',';
        outs << timeReached;
    }
}

void SatisfactionIndex::save() const {
    // 将满意度数据保存到文件
    ofstream result(RESULT_FILENAME);
    printSatisfaction(result, true);
}

ostream& operator<<(ostream& outs, const SatisfactionIndex& index) {
    // 重载输出运算符以输出满意度数据
    index.printRawSatisfaction(outs);
    return outs;
}

istream& operator>>(istream& ins, SatisfactionIndex& index) {
    // 重载输入运算符以读取满意度数据
    index.readRawSatisfaction(ins);
    return ins;
}
