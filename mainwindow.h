#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>

#include<QSpinBox>
#include<QPushButton>
#include<QHBoxLayout>

#include"qt_game/GameController.h"
class BottleWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(GameController* c,QWidget* parent=nullptr);

    void updateGame(const std::vector<std::vector<int>>& bottles);

    //void animatePour(int from,int to);

    void rebuildBoard(int bottleCount);
private:

    std::vector<BottleWidget*> bottleWidgets;
    int capacity;
    int selected =-1;
    GameController* controller;


    QSpinBox* bottleSpin;
    QSpinBox* capacitySpin;
    QSpinBox* colorSpin;

    QPushButton* newBtn;
    QPushButton* resetBtn;
    QPushButton* undoBtn;
    QPushButton* aiBtn;

    QHBoxLayout* gameLayout;

private:
    void onBottleClicked(BottleWidget* b);


};
#endif // MAINWINDOW_H
