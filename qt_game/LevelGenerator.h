#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H


#include"GameState.h"

class LevelGenerator
{
public:
    static void generate(GameState& game,int colorCount);
};

#endif // LEVELGENERATOR_H
