#include <iostream>
#include<QDebug>
#include"GameState.h"
void printGame(const GameState& game)
{
    for(int i=0;i<game.bottles.size();i++)
    {
        std::cout<<"Bottle "<<i<<": ";

        for(int c:game.bottles[i])
        {std::cout<<c<<" ";}
        std::cout<<"\n";
    }

    std::cout<<"-------------\n";
}

#include <QApplication>
#include "GameController.h"
#include "mainwindow.h"
#include"path.h"

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);

    int bottleCount=8;
    int capacity=6;
    int colorCount=7;

    GameController controller(bottleCount,capacity,colorCount);

    MainWindow window(&controller);

    controller.newGame(bottleCount,capacity,colorCount);

    window.updateGame(controller.getGame().bottles);

    QObject::connect(&controller,&GameController::gameUpdated,&window,&MainWindow::updateGame);

    window.show();

    // controller.getGame().saveToJson(levelPath);

    // controller.solveWithPython();

    return app.exec();
}
