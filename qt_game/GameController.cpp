#include "GameController.h"
#include "LevelGenerator.h"
#include<QProcess>

#include<QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include<QCoreApplication> //用于自动退出，QCoreApplication::quit() //only test

GameController::GameController(int bottleCount)
    : game(bottleCount)
{

    solverTimer = new QTimer(this);

    connect(solverTimer,&QTimer::timeout,this,&GameController::stepSolve);

}

void GameController::newGame(int colorCount)
{
    game = GameState(game.bottles.size());

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

bool GameController::solveWithPython()
{
    QProcess process;

    process.setWorkingDirectory("../../rl");

    process.start("python", QStringList() << "solver.py");

    process.waitForFinished();

    qDebug() << process.readAllStandardOutput();

    return true;
}

bool GameController::loadSolution(const QString &path)
{
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
    currentStep=0;
    solverTimer->start(500);
}

void GameController::stepSolve()
{
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
    currentStep++;
}

