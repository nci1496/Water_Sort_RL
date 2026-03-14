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

GameController::GameController(int bottleCount,int capacity,int colorCount)
    : bottleCount(bottleCount),capacity(capacity),colorCount(colorCount),game(bottleCount,capacity,colorCount),initialState(bottleCount,capacity,colorCount)
{

    solverProcess = new QProcess(this);

    solverTimer = new QTimer(this);

    connect(solverTimer,&QTimer::timeout,this,&GameController::stepSolve);

    connect(solverProcess, &QProcess::finished,this, &GameController::onSolverFinished);
    connect(solverProcess,&QProcess::started,[](){qDebug()<<"solverProcess started";});

    connect(solverProcess,&QProcess::readyReadStandardOutput,[=](){qDebug()<<solverProcess->readAllStandardOutput();});

    connect(solverProcess,&QProcess::readyReadStandardError,[=](){qDebug()<<"ERR:"<<solverProcess->readAllStandardError();});

}

void GameController::newGame(int bottleCount,int capacity,int colorCount)
{
    this->bottleCount=bottleCount;
    this->capacity=capacity;
    this->colorCount=colorCount;

    game = GameState(bottleCount,capacity,colorCount);

        while(!history.empty()){history.pop();}

    LevelGenerator::generate(game);
    initialState=game;


}

bool GameController::playerMove(int from,int to)
{
    qDebug()<<"from"<<from<<"size"<<game.bottles[from].size();
    qDebug()<<"to"<<to<<"size"<<game.bottles[to].size();
    qDebug()<<"star playMove()";
    if(!game.canPour(from,to))
    {
        qDebug()<<"can't canPour";
        return false;
    }
    history.push(game);

    game.pour(from,to);

    return true;
}

void GameController::undo()
{
    if(history.empty())
    {
        return;
    }

    game=history.top();
    history.pop();
}

void GameController::reset()
{
    game=initialState;
    while(!history.empty())
    {
        history.pop();
    }
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

const GameState &GameController::getState() const
{
    return game;
}

int GameController::getBottleCount()
{
    return bottleCount;
}

int GameController::getCapacity()
{
    return capacity;
}

int GameController::getColorCount()
{
    return colorCount;
}

// void GameController::setBottleCount(int bottleCount)
// {
//     this->bottleCount=bottleCount;
// }

// void GameController::setCapacity(int capacity)
// {
//     this->capacity=capacity;
// }

// void GameController::setColorCount(int colorCount)
// {
//     this->colorCount=colorCount;
// }


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

