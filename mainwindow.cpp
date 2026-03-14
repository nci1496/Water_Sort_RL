#include "mainwindow.h"
#include "qt_game/BottleWidget.h"
#include <QHBoxLayout>

#include<QLabel>

MainWindow::MainWindow(GameController* c,QWidget* parent)
    : QWidget(parent)
{
    controller = c;

    this->capacity=c->getCapacity();

    QVBoxLayout* mainLayout = new QVBoxLayout();

    QHBoxLayout* controlLayout = new QHBoxLayout();

    bottleSpin = new QSpinBox();
    capacitySpin = new QSpinBox();
    colorSpin = new QSpinBox();

    bottleSpin->setRange(3,20);
    capacitySpin->setRange(2,8);
    colorSpin->setRange(2,15);

    bottleSpin->setValue(controller->getBottleCount());
    capacitySpin->setValue(controller->getCapacity());
    colorSpin->setValue(controller->getColorCount());
    bottleSpin->setFixedWidth(60);
    capacitySpin->setFixedWidth(60);
    colorSpin->setFixedWidth(60);

    newBtn = new QPushButton("New Level");
    resetBtn = new QPushButton("Reset");
    undoBtn = new QPushButton("Undo");
    aiBtn = new QPushButton("AI Solve");

    controlLayout->addWidget(new QLabel("Bottles"));
    controlLayout->addWidget(bottleSpin);

    controlLayout->addWidget(new QLabel("Capacity"));
    controlLayout->addWidget(capacitySpin);

    controlLayout->addWidget(new QLabel("Colors"));
    controlLayout->addWidget(colorSpin);

    controlLayout->addWidget(newBtn);
    controlLayout->addWidget(resetBtn);
    controlLayout->addWidget(undoBtn);
    controlLayout->addWidget(aiBtn);

    gameLayout = new QHBoxLayout();
    gameLayout->setSpacing(15);
    gameLayout->setContentsMargins(20,20,20,20);

    for(int i=0;i<c->getBottleCount();i++)
    {
        BottleWidget* b = new BottleWidget();

        b->setCapacity(capacity);
        bottleWidgets.push_back(b);

        gameLayout->addWidget(b);
        connect(b,&BottleWidget::clicked,this,&MainWindow::onBottleClicked);
    }

    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(gameLayout);

    setLayout(mainLayout);

    resize(500,250);

    connect(newBtn,&QPushButton::clicked,this,[=]()
            {
        int bottle = bottleSpin->value();
        int cap = capacitySpin->value();
        int color = colorSpin->value();

        if(color>=bottle){color=bottle-1;colorSpin->setValue(color);}

        controller->newGame(bottle,cap,color);

        rebuildBoard(bottle);

        updateGame(controller->getState().bottles);
    });

    connect(resetBtn,&QPushButton::clicked,this,[=]()
            {
                controller->reset();
                updateGame(controller->getState().bottles);
            });

    connect(undoBtn,&QPushButton::clicked,this,[=]()
            {
                controller->undo();
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
            idx = i;
            break;
        }
    }

    if(idx==-1) return;

    // 第一次点击
    if(selected==-1)
    {
        selected = idx;
        bottleWidgets[idx]->setSelected(true);
        return;
    }

    // 点击同一个瓶子 → 取消选择
    if(selected == idx)
    {
        bottleWidgets[idx]->setSelected(false);
        selected = -1;
        return;
    }

    // 第二次点击 → 尝试倒水
    controller->playerMove(selected,idx);

    // 无论成功失败，都取消高亮
    bottleWidgets[selected]->setSelected(false);

    selected = -1;

    updateGame(controller->getState().bottles);
}

void MainWindow::rebuildBoard(int bottleCount)
{
    for(auto b : bottleWidgets)
    {
        gameLayout->removeWidget(b);
        delete b;
    }

    bottleWidgets.clear();

    for(int i=0;i<bottleCount;i++)
    {
        BottleWidget* b = new BottleWidget();

        b->setCapacity(capacitySpin->value());

        bottleWidgets.push_back(b);

        gameLayout->addWidget(b);

        connect(b,&BottleWidget::clicked,this,&MainWindow::onBottleClicked);
    }
}
