#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameState.h"
#include<QTimer>
#include<QProcess>

#include<stack>

class GameController: public QObject //
{
    Q_OBJECT //不加继承自QObject的话，用不了connect
public:

GameController(int bottleCount,int capacity,int colorCount);
//游戏核心流程
    void newGame(int bottleCount,int capacity,int colorCount);
    bool playerMove(int from,int to);
    void undo();
    void reset();
//调用.py
    void solveWithPython();
//用json读解法
    bool loadSolution(const QString & path);

    void executeSolution();//一次性直接执行完
    void startAutoSolve();//一步一步来，快慢受solverTimer控制

//读信息
    const GameState& getState() const;
    const GameState& getGame() const;
    int getBottleCount();
    int getCapacity();
    int getColorCount();    
// //改参数    //不需要提供改参数接口，因为我要改，基本上都是新建游戏的时候，我直接在新建游戏处改，下面的initialState,game,history都改了
//     void setBottleCount(int bottleCount);
//     void setCapacity(int capacity);
//     void setColorCount(int colorCount);

private slots:
    void stepSolve();
private:
//用于求解动画
    QTimer*solverTimer;
    int currentStep=0;
//用于py求解
    QProcess* solverProcess;
    void  onSolverFinished();
//存解法
    std::vector<std::pair<int,int>> solution;

//关卡属性
    int bottleCount;
    int capacity;
    int colorCount;

//关卡信息
    GameState initialState;
    std::stack<GameState> history;

//游戏属性
    GameState game;


signals:
    void gameUpdated(const std::vector<std::vector<int>>& bottles);
    //void animatePourSignal(int from ,int to);弃用倾斜倒水动画
};

#endif // GAMECONTROLLER_H
