#ifndef GAMESTATE_H
#define GAMESTATE_H

#include<vector>
#include<QString>

class GameState
{
public:
    GameState(int bottleCount,int capacity);

    bool canPour(int from,int to) const;

    void pour(int from,int to);

    bool isSolved() const;

    int topColor(int bottle) const;

    int emptySpace(int bottle) const;

    int capacity;
    int bottleCount;

    std::vector<std::vector<int>> bottles;

    //导出关卡
    bool saveToJson(const QString& path) const;
    bool loadFromJson(const QString& path);
};

#endif // GAMESTATE_H
