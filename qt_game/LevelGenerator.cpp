#include "LevelGenerator.h"
#include <algorithm>
#include <random>

#include<QDebug>

// void LevelGenerator::generate(GameState& game,int colorCount)
// {
//     std::vector<int> pool;

//     for(int c=1;c<=colorCount;c++)
//         for(int i=0;i<game.capacity;i++)
//             pool.push_back(c);

//     std::random_device rd;
//     std::mt19937 g(rd());

//     std::shuffle(pool.begin(),pool.end(),g);

//     int idx=0;

//     for(int i=0;i<colorCount;i++)
//     {
//         for(int j=0;j<game.capacity;j++)
//         {
//             game.bottles[i].push_back(pool[idx++]);
//         }
//     }
//     for(auto& b : game.bottles)
//     {
//         if(b.size() > game.capacity)
//             qDebug()<<"ERROR bottle overflow";
//     }
//     if(colorCount > game.bottles.size())
//     {
//         qDebug()<<"Too many colors!";
//         return;
//     }

//     // 剩余瓶子保持为空
// }

void LevelGenerator::generate(GameState& game)
{
    int bottleCount = game.bottleCount;
    int capacity = game.capacity;

    std::random_device rd;
    std::mt19937 rng(rd());

    while(true)
    {
        for(auto& b : game.bottles)
            b.clear();

        // 终局
        for(int c=0;c<game.colorCount;c++)
        {
            for(int i=0;i<capacity;i++)
                game.bottles[c].push_back(c+1);
        }

        int shuffleSteps = 1000;

        for(int step=0;step<shuffleSteps;step++)
        {
            int from = rng()%bottleCount;
            int to   = rng()%bottleCount;

            if(from==to)
                continue;

            auto& A = game.bottles[from];
            auto& B = game.bottles[to];

            if(A.empty())
                continue;

            if(B.size()>=capacity)
                continue;

            // 倒1格（逆规则）
            B.push_back(A.back());
            A.pop_back();
        }

        if(!game.isSolved())
            break;
    }
}
