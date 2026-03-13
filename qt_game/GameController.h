#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameState.h"
#include<QTimer>
#include<QProcess>

class GameController: public QObject //
{
    Q_OBJECT //不加继承自QObject的话，用不了connect
public:

    GameController(int bottleCount,int capacity);

    void newGame(int colorCount);

    bool playerMove(int from,int to);

    const GameState& getGame() const;

    void solveWithPython();

    bool loadSolution(const QString & path);

    void executeSolution();//一次性直接执行完
    void startAutoSolve();//一步一步来，快慢受solverTimer控制


private slots:
    void stepSolve();
private:
    //用于求解动画
    QTimer*solverTimer;
    int currentStep=0;

    std::vector<std::pair<int,int>> solution;

    int bottleCount;
    int capacity;


    GameState game;
    QProcess* solverProcess;

    void  onSolverFinished();
signals:
    void gameUpdated(const std::vector<std::vector<int>>& bottles);
    //void animatePourSignal(int from ,int to);弃用倾斜倒水动画
};

#endif // GAMECONTROLLER_H
