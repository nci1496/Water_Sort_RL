#include "GameState.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include<QFileInfo>
#include<QDir>

#include<QDebug> //only test

GameState::GameState(int bottleCount,int capacity,int colorCount)
{

    this->bottleCount=bottleCount;
    this->capacity=capacity;
    this->colorCount=colorCount;

    bottles.resize(bottleCount);
}

int GameState::topColor(int bottle) const
{
    if(bottles[bottle].empty())
        return -1;

    return bottles[bottle].back();
}

int GameState::emptySpace(int bottle) const
{
    return capacity - bottles[bottle].size();
}

bool GameState::canPour(int from,int to) const
{
    if(from == to)
    {
        qDebug()<<"1"<<Qt::endl;
        return false;
    }
    if(bottles[from].empty())
    {
                qDebug()<<"2"<<Qt::endl;
        return false;
    }
    if(bottles[to].size() >= capacity)
    {
                qDebug()<<"3"<<Qt::endl;
        return false;
    }
    int color = topColor(from);

    if(!bottles[to].empty() && topColor(to) != color)
    {
                qDebug()<<"4"<<Qt::endl;
        return false;
    }
    return true;
}

void GameState::pour(int from,int to)
{
    if(!canPour(from,to))
    {
        qDebug()<<"Invaild:"<<from<<"to"<<to<<Qt::endl;
        return;
    }
    int color = topColor(from);

    while(!bottles[from].empty())
    {
        if(bottles[from].back() != color)
            break;

        if(bottles[to].size() >= capacity)
            break;

        bottles[to].push_back(color);
        bottles[from].pop_back();
    }
}

bool GameState::isSolved() const
{
    for(const auto &b : bottles)
    {
        if(b.empty())
            continue;

        if(b.size() != capacity)
            return false;

        int color = b[0];

        for(int c : b)
        {
            if(c != color)
                return false;
        }
    }

    return true;
}

bool GameState::saveToJson(const QString& path) const
{
    QFile file(path);

    QDir dir = QFileInfo(file).dir();
    if(!dir.exists())
        dir.mkpath(".");

    if(!file.open(QIODevice::WriteOnly))
        return false;

    QJsonArray bottlesArray;

    for(const auto& bottle : bottles)
    {
        QJsonArray bottleArray;

        for(int c : bottle)
            bottleArray.append(c);

        bottlesArray.append(bottleArray);
    }

    QJsonObject root;

    root["capacity"] = capacity;

    QJsonArray bottleArray;

    for(const auto& b : bottles)
    {
        QJsonArray arr;

        for(int c : b)
            arr.append(c);

        bottleArray.append(arr);
    }

    root["bottles"] = bottlesArray;

    QJsonDocument doc(root);

    file.write(doc.toJson());

    file.close();

    return true;
}

bool GameState::loadFromJson(const QString& path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
    {return false;}
    QByteArray data = file.readAll();

    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if(!doc.isObject())
    {return false;
    }
    QJsonObject root = doc.object();

    QJsonArray bottlesArray = root["bottles"].toArray();

    bottles.clear();

    for(auto bottleValue : bottlesArray)
    {
        QJsonArray bottleArray = bottleValue.toArray();

        std::vector<int> bottle;

        for(auto c : bottleArray)
        {bottle.push_back(c.toInt());}
        bottles.push_back(bottle);
    }

    return true;
}
