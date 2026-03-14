#include "mainwindow.h"
#include "qt_game/BottleWidget.h"
#include <QHBoxLayout>

#include<QPushButton>

MainWindow::MainWindow(GameController* c,QWidget* parent)
    : QWidget(parent)
{
    controller = c;

    this->capacity=c->getCapacity();

    QVBoxLayout* root = new QVBoxLayout();

    QHBoxLayout* buttons = new QHBoxLayout();

    QPushButton* newBtn = new QPushButton("New Level");
    QPushButton* resetBtn = new QPushButton("Reset");
    QPushButton* undoBtn = new QPushButton("Undo");
    QPushButton* aiBtn = new QPushButton("AI Solve");

    buttons->addWidget(newBtn);
    buttons->addWidget(resetBtn);
    buttons->addWidget(undoBtn);
    buttons->addWidget(aiBtn);

    QHBoxLayout* bottleLayout = new QHBoxLayout();
    bottleLayout->setSpacing(15);
    bottleLayout->setContentsMargins(20,20,20,20);

    for(int i=0;i<c->getBottleCount();i++)
    {
        BottleWidget* b = new BottleWidget();

        b->setCapacity(capacity);
        bottleWidgets.push_back(b);

        bottleLayout->addWidget(b);
        connect(b,&BottleWidget::clicked,this,&MainWindow::onBottleClicked);
    }

    root->addLayout(buttons);
    root->addLayout(bottleLayout);

    setLayout(root);

    resize(500,250);

    connect(newBtn,&QPushButton::clicked,this,[=]()
            {
        controller->newGame();
                updateGame(controller->getState().bottles);
            });

    connect(resetBtn,&QPushButton::clicked,this,[=]()
            {
                //controller->reset();
                updateGame(controller->getState().bottles);
            });

    connect(undoBtn,&QPushButton::clicked,this,[=]()
            {
                //controller->undo();
                updateGame(controller->getState().bottles);
            });

    connect(aiBtn,&QPushButton::clicked,this,[=]()
            {
                qDebug()<<"AI solving...";//暂时没实现
            });

    updateGame(controller->getState().bottles);
}

void MainWindow::updateGame(const std::vector<std::vector<int>>& bottles)
{
    for(int i=0;i<bottles.size();i++)
    {
        bottleWidgets[i]->setBottle(bottles[i]);
    }
}

void MainWindow::onBottleClicked(BottleWidget* b)
{
    int idx = -1;

    for(int i=0;i<bottleWidgets.size();i++)
    {
        if(bottleWidgets[i]==b)
        {
            idx=i;
            break;
        }
    }

    if(idx==-1) {return;}
    // 第一次点击
    if(selected==-1)
    {
        selected=idx;
        return;
    }

    // 第二次点击
    controller->playerMove(selected,idx);

    selected=-1;

    updateGame(controller->getState().bottles);
}
