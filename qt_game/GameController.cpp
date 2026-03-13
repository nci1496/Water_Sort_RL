#include "GameController.h"
#include "LevelGenerator.h"
#include<QProcess>

#include<QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include"path.h"

#include<QCoreApplication> //用于自动退出，QCoreApplication::quit() //only test

GameController::GameController(int bottleCount,int capacity)
    : game(bottleCount,capacity),bottleCount(bottleCount),capacity(capacity)
{
    solverProcess = new QProcess(this);

    solverTimer = new QTimer(this);

    connect(solverTimer,&QTimer::timeout,this,&GameController::stepSolve);

    connect(solverProcess, &QProcess::finished,this, &GameController::onSolverFinished);
    connect(solverProcess,&QProcess::started,[](){qDebug()<<"solverProcess started";});

    connect(solverProcess,&QProcess::readyReadStandardOutput,[=](){qDebug()<<solverProcess->readAllStandardOutput();});

    connect(solverProcess,&QProcess::readyReadStandardError,[=](){qDebug()<<"ERR:"<<solverProcess->readAllStandardError();});

}

void GameController::newGame(int colorCount)
{
    game = GameState(bottleCount,capacity);

    LevelGenerator::generate(game,colorCount);
}

bool GameController::playerMove(int from,int to)
{
    if(!game.canPour(from,to))
    {return false;}
    game.pour(from,to);

    return true;
}

const GameState& GameController::getGame() const
{
    return game;
}

void GameController::solveWithPython()
{
    QString program = "python";

    QStringList args;
    args << pyPath << levelPath << solutionPath;

    qDebug()<<"start python:";
    qDebug()<<program;
    qDebug()<<args;

    solverProcess->start(program,args);

    if(!solverProcess->waitForStarted())
    {
        qDebug()<<"Python start failed";
    }
}

bool GameController::loadSolution(const QString &path)
{
    qDebug()<<"Loading solution from :"<<path<<Qt::endl;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
    {return false;}
    QByteArray data = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject())
    {return false;}
    QJsonObject root = doc.object();

    QJsonArray movesArray = root["moves"].toArray();

    solution.clear();

    for(auto m : movesArray)
    {
        QJsonArray move = m.toArray();

        int from = move[0].toInt();
        int to   = move[1].toInt();

        solution.push_back({from,to});
    }

    return true;
}

void GameController::executeSolution()
{
    for(auto move : solution)
    {
        int from = move.first;
        int to   = move.second;

        if(game.canPour(from,to))
        {
            game.pour(from,to);

            qDebug()<<"Move"<<from<<"->"<<to;
        }
        else
        {
            qDebug()<<"Invalid move"<<from<<"->"<<to;
        }
    }
}

void GameController::startAutoSolve()
{
    //currentStep=0;
    //solverTimer->start(500);
}

void GameController::stepSolve()
{
    if(solution.empty())
    {
        qDebug()<<"No solution loaded";
        return;
    }

    if(currentStep >= solution.size())
    {
        solverTimer->stop();
        qDebug()<<"Solved!";

        //QCoreApplication::quit();   // 退出程序  //only test
        return;
    }

    int from = solution[currentStep].first;
    int to   = solution[currentStep].second;

    if(game.canPour(from,to))
    {
        game.pour(from,to);

        qDebug()<<"Move"<<from<<"->"<<to;
    }
    emit gameUpdated(game.bottles);//更新UI
    // emit animatePourSignal(from,to);//弃用了倾斜倒水动画
    // QTimer::singleShot(350,[this,from,to](){game.pour(from,to);emit gameUpdated(game.bottles);});
    currentStep++;
}

void GameController::onSolverFinished()
{
    qDebug()<<"Python finished";

    if(loadSolution(solutionPath))
    {
        qDebug()<<"solution steps:"<<solution.size();

        currentStep = 0;
        solverTimer->start(400);
    }
    else
    {
        qDebug()<<"load solution failed";
    }
    bool ok = loadSolution(solutionPath);

    qDebug()<<"loadSolution result:"<<ok;
    qDebug()<<"solution size:"<<solution.size();
}

