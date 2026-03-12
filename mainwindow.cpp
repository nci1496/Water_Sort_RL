#include "MainWindow.h"
#include "qt_game/BottleWidget.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();

    for(int i=0;i<6;i++)
    {
        BottleWidget* b = new BottleWidget();

        bottleWidgets.push_back(b);

        layout->addWidget(b);
    }

    setLayout(layout);

    resize(500,250);
}

void MainWindow::updateGame(const std::vector<std::vector<int>>& bottles)
{
    for(int i=0;i<bottles.size();i++)
    {
        bottleWidgets[i]->setBottle(bottles[i]);
    }
}
