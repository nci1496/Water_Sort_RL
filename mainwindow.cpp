#include "mainwindow.h"
#include "qt_game/BottleWidget.h"
#include <QHBoxLayout>

MainWindow::MainWindow(int bottleCount,int capacity,QWidget* parent)
    : QWidget(parent)
{
    this->capacity=capacity;
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(15);
    layout->setContentsMargins(20,20,20,20);

    for(int i=0;i<bottleCount;i++)
    {
        BottleWidget* b = new BottleWidget();

        b->setCapacity(capacity);
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

// void MainWindow::animatePour(int from,int to)
// {
//     BottleWidget* b1 = bottleWidgets[from];

//     QPropertyAnimation* anim = new QPropertyAnimation(b1,"angle");

//     anim->setDuration(300);

//     anim->setStartValue(0);
//     anim->setEndValue(-40);

//     anim->setEasingCurve(QEasingCurve::InOutQuad);

//     connect(anim,&QPropertyAnimation::finished,[=]()
//             {
//                 QPropertyAnimation* back = new QPropertyAnimation(b1,"angle");

//                 back->setDuration(300);

//                 back->setStartValue(-40);
//                 back->setEndValue(0);

//                 back->start();
//             });

//     anim->start();
// }
