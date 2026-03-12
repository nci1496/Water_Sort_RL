#include "LevelGenerator.h"
#include <algorithm>
#include <random>

void LevelGenerator::generate(GameState& game,int colorCount)
{
    std::vector<int> pool;

    for(int c=1;c<=colorCount;c++)
        for(int i=0;i<GameState::CAPACITY;i++)
            pool.push_back(c);

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(pool.begin(),pool.end(),g);

    int idx=0;

    for(int i=0;i<colorCount;i++)
    {
        for(int j=0;j<GameState::CAPACITY;j++)
        {
            game.bottles[i].push_back(pool[idx++]);
        }
    }

    // 剩余瓶子保持为空
}
