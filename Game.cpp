#include <random>
#include <sstream>
#include "Game.h"
#include "AI.h"
#include "Utility.h"

using namespace std;

// 游戏主循环函数，如果AI模式被激活，使用AI进行游戏
void Game::playGame(bool isAIModeIn, ifstream &gameFile) {
    if (!gameFile.is_open()) {
        exit(1);  // 文件未打开，退出程序
    }
    isAIMode = isAIModeIn;
    printGameStartPrompt();  // 打印游戏开始提示
    initGame(gameFile);  // 初始化游戏
    string personString;
    Person p;
    while (gameFile >> personString) {
        p = Person(personString);
        int personTick = p.getTurn();
        while (personTick > building.getTime()) {
            building.prettyPrintBuilding(cout);  // 优雅地打印建筑信息
            satisfactionIndex.printSatisfaction(cout, false);  // 打印满意度指数
            checkForGameEnd();  // 检查游戏是否结束
            Move nextMove = getMove();  // 获取下一个动作
            update(nextMove);  // 更新游戏状态
        }
        building.spawnPerson(p);  // 在建筑中生成人物
    }
    while (true) {
        building.prettyPrintBuilding(cout);  // 持续更新和打印游戏状态
        satisfactionIndex.printSatisfaction(cout, false);
        checkForGameEnd();
        Move nextMove = getMove();
        update(nextMove);
    }
}

// 验证是否为有效的乘客列表
bool Game::isValidPickupList(const string &pickupList, const int pickupFloorNum) const {
    int numPeople = pickupList.length();
    int max = 0;
    int currentPersonIndex;
    int personFloorTarget;
    int personList[numPeople];
    for (int i = 0; i < numPeople; i++) {
        personList[i] = pickupList[i] - '0';
    }
    for (int i = 0; i < numPeople; i++) {
        for (int j = i + 1; j < numPeople; j++) {
            if (personList[i] == personList[j]) {
                return false;  // 确保每个人都是独一无二的
            }
        }
        if (!(personList[i] >= 0 && personList[i] <= 9)) {
            return false;  // 确保所有的字符都是数字
        }
        if (personList[i] > max) {
            max = personList[i];
        }
    }
    if (max >= building.getFloorByFloorNum(pickupFloorNum).getNumPeople()) {
        return false;  // 确保没有超出最大人数
    }
    if (!(numPeople <= ELEVATOR_CAPACITY)) {
        return false;  // 确保不超过电梯容量
    }
    bool isDirectionUp;
    currentPersonIndex = personList[0];
    personFloorTarget = building.getFloorByFloorNum(pickupFloorNum)
            .getPersonByIndex(currentPersonIndex)
            .getTargetFloor();
    if (personFloorTarget > pickupFloorNum) {
        isDirectionUp = true;
    } else if (personFloorTarget < pickupFloorNum) {
        isDirectionUp = false;
    }
    for (int i = 1; i < numPeople; i++) {
        currentPersonIndex = personList[i];
        if (isDirectionUp) {
            if (building.getFloorByFloorNum(pickupFloorNum)
                        .getPersonByIndex(currentPersonIndex)
                        .getTargetFloor() < pickupFloorNum) {
                return false;  // 如果上行，则目标楼层必须高于当前楼层
            }
        } else {
            if (building.getFloorByFloorNum(pickupFloorNum)
                        .getPersonByIndex(currentPersonIndex)
                        .getTargetFloor() > pickupFloorNum) {
                return false;  // 如果下行，则目标楼层必须低于当前楼层
            }
        }
    }
    return true;
}

// 执行移动
bool Game::performMove(Move &move) const {
    Elevator elevators[NUM_ELEVATORS];
    for (int i = 0; i < NUM_ELEVATORS; ++i) {
        elevators[i] = building.getElevatorById(i);
    }
    if (move.isValidMove(elevators) && move.isPickupMove()) {
        Elevator taggedElevator = building.getElevatorById(move.getElevatorId());
        Floor taggedFloor = building.getFloorByFloorNum(taggedElevator.getCurrentFloor());
        if (taggedFloor.getNumPeople() > 0) {
            getPeopleToPickup(move);  // 确认乘客挑选
            return true;
        }
        return false;
    } else if (move.isSaveMove()) {
        performSave();  // 执行保存操作
    } else if (move.isQuitMove()) {
        performQuit();  // 执行退出操作
    } else if (!move.isValidMove(elevators)) {
        return false;  // 如果不是有效移动，返回false
    }
    return true;
}

// 打印欢迎菜单
void Game::printWelcomeMenu() const {
    cout << "########################################" << endl;
    cout << "# Welcome to EECS 183 Elevator Escape! #" << endl;
    cout << "########################################" << endl << endl;
}

// 打印介绍菜单
void Game::printIntroMenu() const {
    cout << "[0] Run Tests" << endl;
    cout << "[1] Load saved game" << endl;
    cout << "[2] Start new game" << endl;
    cout << "Choice: ";
}

// 打印AI菜单
void Game::printAIMenu() const {
    cout << "[1] Play as human" << endl;
    cout << "[2] Watch AI play" << endl;
    cout << "Choice: ";
}

// 打印选项
void Game::printOptions() const {
    cout << "########################################" << endl;
    cout << "#         Enter service move:          #" << endl;
    cout << "#     'e' elevatorId 'f' floorNum      #" << endl;
    cout << "#            Or pickup move:           #" << endl;
    cout << "#          'e' elevatorId 'p'          #" << endl;
    cout << "########################################" << endl;
}

// 打印游戏开始提示信息
void Game::printGameStartPrompt() const {
    cout << endl;
    cout << "########################################" << endl;
    cout << "#                 Enter:               #" << endl;
    cout << "#               Q to Quit              #" << endl;
    cout << "#               S to Save              #" << endl;
    cout << "#      At any time after this prompt   #" << endl;
    cout << "########################################" << endl;
    cout << endl;
    cout << "Press enter to continue....";  // 提示用户按回车键继续
    string trash;
    getline(cin, trash);  // 读取并丢弃用户的输入
    cout << endl;
}

// 获取游戏开始的选择
IntroChoice Game::getIntroChoice() const {
    if (IS_AI_OVERRIDE) {
        return IntroChoice::New;  // 如果有AI覆盖，直接返回新游戏选项
    }
    int choice = -1;
    while ((choice < 0) || (choice > 2)) {
        printIntroMenu();  // 打印介绍菜单
        string s_choice;
        getline(cin, s_choice);  // 从用户输入中获取选择
        choice = stoi(s_choice);  // 将字符串转换为整数
        if ((choice < 0) || (choice > 2)) {
            cout << endl << "Invalid menu choice!" << endl << endl;  // 如果选择无效，打印错误信息
        }
    }
    cout << endl;
    return static_cast<IntroChoice>(choice);  // 返回类型转换后的选择
}

// 获取AI模式的选择
AIChoice Game::getAIChoice() const {
    if (IS_AI_OVERRIDE) {
        return AIChoice::AI;  // 如果有AI覆盖，直接返回AI模式
    }
    int choice = 0;
    printAIMenu();  // 打印AI菜单
    string s_choice;
    getline(cin, s_choice);  // 从用户输入中获取选择
    choice = stoi(s_choice);  // 将字符串转换为整数
    while (choice != 1 && choice != 2) {
        cout << endl << "Invalid menu choice!" << endl << endl;  // 如果选择无效，打印错误信息
        getline(cin, s_choice);  // 重新从用户输入中获取选择
        choice = stoi(s_choice);  // 再次将字符串转换为整数
    }
    cout << endl;
    return static_cast<AIChoice>(choice);  // 返回类型转换后的选择
}

// 打印满意度指数
void Game::printSatisfactionIndex() const {
    cout << endl << endl;
    cout << "Your hotel's current satisfaction index is: " << satisfactionIndex;  // 打印当前的满意度指数
    cout << endl << endl;
}

// 获取需要接送的人员
void Game::getPeopleToPickup(Move &move) const {
    int currentFloorNum = building.getElevatorById(move.getElevatorId()).getCurrentFloor();  // 获取当前电梯所在楼层
    Floor currentFloor = building.getFloorByFloorNum(currentFloorNum);  // 获取当前楼层对象
    if (currentFloor.getNumPeople() == 1) {
        move.setPeopleToPickup("0", currentFloorNum, currentFloor);  // 如果楼层只有一个人，直接设置为接送对象
        return;
    }
    string peopleIndices = "";
    bool isValidPickup = false;
    while (!isValidPickup) {
        currentFloor.printFloorPickupMenu(cout);  // 打印楼层的接送菜单
        cout << endl;
        cout << "Choices: ";
        getline(cin, peopleIndices);  // 获取用户的接送选择
        cout << "\n";
        if (isAIMode) {
            Elevator e = building.getElevatorById(move.getElevatorId());
            Floor f = building.getFloorByFloorNum(e.getCurrentFloor());
            peopleIndices = getAIPickupList(move, building.getBuildingState(), f);  // 如果是AI模式，获取AI的接送列表
            cout << "AI OVERRIDE: " << peopleIndices << endl;  // 打印AI的选择
        }
        isValidPickup = isValidPickupList(peopleIndices, currentFloorNum);  // 验证用户选择的有效性
        if (!isValidPickup) {
            cout << "Invalid list of people chosen!" << endl;  // 如果无效，打印错误信息
            checkForGameEnd();  // 检查游戏是否结束
        }
    }
    move.setPeopleToPickup(peopleIndices, currentFloorNum, currentFloor);  // 设置有效的接送人员
}

// 获取玩家的移动
Move Game::getMove() const {
    Move moveObj;
    string moveString = "";
    bool isValid = false;
    GameChoice choiceCapture = GameChoice::Invalid;
    do {
        cout << "Enter move: ";
        getline(cin, moveString);  // 获取用户输入的移动
        cout << "\n";
        if (isAIMode) {
            moveString = getAIMoveString(building.getBuildingState());  // 如果是AI模式，获取AI的移动
            cout << "AI OVERRIDE: " << moveString << endl;  // 打印AI的移动
        }
        moveObj = Move(moveString);  // 创建移动对象
        choiceCapture = static_cast<GameChoice>(moveString[0]);  // 获取移动的类型
        switch (choiceCapture) {
            case GameChoice::Quit:
                performQuit();  // 执行退出操作
                break;
            case GameChoice::Save:
                isValid = performSave();  // 执行保存操作
                break;
            case GameChoice::Pass:
            case GameChoice::Move:
                isValid = performMove(moveObj);  // 执行移动操作
                break;
            default:
                isValid = false;  // 其他未知命令视为无效
                break;
        }
        if (!isValid) {
            cout << endl << "Invalid move!" << endl << endl;  // 如果移动无效，打印错误信息
            checkForGameEnd();  // 检查游戏是否结束
        }
    } while (!isValid);
    return moveObj;  // 返回有效的移动对象
}

// 执行退出操作
void Game::performQuit() const {
    QuitChoice quitChoice = static_cast<QuitChoice>(getExitChoice());  // 获取退出选择
    switch (quitChoice) {
        case QuitChoice::Quit:
            endGame();  // 结束游戏
            break;
        case QuitChoice::Stay:
            return;  // 继续游戏
            break;
    }
}

// 执行保存操作
bool Game::performSave() const {
    ofstream saveFile(LOAD_FILENAME);  // 打开保存文件
    if (!saveFile.is_open()) {
        cout << "Could not open " << LOAD_FILENAME << endl;  // 如果文件无法打开，打印错误信息
        return false;
    }
    return saveGame(saveFile);  // 保存游戏状态
}

// 初始化游戏
void Game::initGame(ifstream &loadFile) {
    cout << endl << "Loading game..." << endl;  // 打印加载游戏的信息
    loadFile >> satisfactionIndex;  // 读取满意度指数
    int tempTime = 0;
    loadFile >> tempTime;  // 读取游戏时间
    building.setTime(tempTime);  // 设置建筑的时间
    string elvState;
    int i = 0;
    while (i < NUM_ELEVATORS && loadFile >> elvState) {
        building.setElevator(elvState, i);  // 设置电梯状态
        i++;
    }
    if (!loadFile.fail()) {
        cout << "Loaded!" << endl << endl;  // 如果加载成功，打印成功信息
    } else {
        cout << endl << "Error when loading the file! "
             << "Verify that you have correctly created your project."
             << endl << endl;
        cout << "You will need to fix this before implementing Game::playGame "
             << "or the Reach AI." << endl << endl;
        cout << "Press enter to continue....";  // 如果加载失败，提示错误并要求用户按回车继续
        string junk;
        getline(cin, junk);
        cout << endl;
    }
    return;
}

// 打印退出菜单
void Game::printExitMenu() const {
    cout << endl;
    cout << "Are you sure you want to quit?" << endl;  // 确认用户是否真的想要退出
    cout << "[1] Yes, I'm sure!" << endl;  // 确认退出
    cout << "[2] No, keep playing!" << endl;  // 继续游戏
    cout << "Choice: ";  // 提示用户做出选择
}

// 获取退出选择
int Game::getExitChoice() const {
    int exitChoice = 0;
    while ((exitChoice < 1) || (exitChoice > 2)) {
        printExitMenu();  // 打印退出菜单
        string choice = "";
        getline(cin, choice);  // 获取用户的选择
        exitChoice = stoi(choice);  // 将字符串转换为整数
        if ((exitChoice < 1) || (exitChoice > 2)) {
            cout << endl << "Invalid menu choice!" << endl << endl;  // 如果选择无效，打印错误信息
        }
    }
    cout << endl;
    return exitChoice;  // 返回退出选择
}

// 结束游戏
void Game::endGame() const {
    cout << endl << "Goodbye!" << endl;  // 打印再见信息
    satisfactionIndex.save();  // 保存满意度指数
    exit(0);  // 退出程序
}

// 打印成功结束的信息
void Game::printSuccessEnding() const {
    cout << endl << endl;
    cout << "Time is up! Your final score is: " << endl;  // 打印时间到，展示最终得分
    satisfactionIndex.printSatisfaction(cout);  // 打印满意度指数
}

// 打印失败结束的信息
void Game::printFailureEnding() const {
    cout << endl << endl;
    cout << "Uh oh! Your hotel has gone out of business!" << endl;  // 打印酒店破产的信息
    cout << "Better luck next time!" << endl << endl;
    cout << "Your final achievement:" << endl << endl;  // 展示最终成就
    satisfactionIndex.printSatisfaction(cout);  // 打印满意度指数
}

// 检查游戏是否结束
void Game::checkForGameEnd() const {
    if (building.getTime() >= MAX_TURNS) {
        printSuccessEnding();  // 如果达到最大轮次，打印成功结束的信息
        endGame();  // 结束游戏
    } else if (satisfactionIndex.getSatisfaction() < 0) {
        printFailureEnding();  // 如果满意度低于0，打印失败结束的信息
        endGame();  // 结束游戏
    }
}

// 更新游戏状态
void Game::update(const Move &m) {
    if (m.isPickupMove()) {
        satisfactionIndex.updateSumDirectionRequest(m, building);  // 更新根据方向请求的满意度
    }
    satisfactionIndex.updateSumExploded(building.tick(m));  // 更新因爆炸损失的满意度
    satisfactionIndex.updateTimeReached(building);  // 更新到达时间的满意度
    if (building.getTime() % 2) {
        satisfactionIndex.updateSumIdle(building);  // 更新闲置时间的满意度
    }
}

// 保存游戏状态
bool Game::saveGame(ofstream &saveFile) const {
    return true;  // 返回保存成功
}
