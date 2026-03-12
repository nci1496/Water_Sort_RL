#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameState.h"
#include<QTimer>


class GameController: public QObject //
{
    Q_OBJECT //不加继承自QObject的话，用不了connect
public:

    GameController(int bottleCount);

    void newGame(int colorCount);

    bool playerMove(int from,int to);

    const GameState& getGame() const;

    bool solveWithPython();

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

    GameState game;

signals:
    void gameUpdated(const std::vector<std::vector<int>>& bottles);

};

#endif // GAMECONTROLLER_H
