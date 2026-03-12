#include <iostream>
#include "GameController.h"
#include<QDebug>

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

int main(int argc,char* argv[])
{
    QApplication app(argc,argv);

    GameController controller(6);

    MainWindow window;

    controller.newGame(4);

    window.updateGame(controller.getGame().bottles);

    QObject::connect(&controller,&GameController::gameUpdated,&window,&MainWindow::updateGame);

    window.show();

    controller.getGame().saveToJson("../../data/level.json");

    controller.solveWithPython();

    controller.loadSolution("../../data/solution.json");

    controller.startAutoSolve();

    return app.exec();
}
