#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>

class BottleWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(int bottleCount,int capacity,QWidget* parent=nullptr);

    void updateGame(const std::vector<std::vector<int>>& bottles);

    //void animatePour(int from,int to);

private:

    std::vector<BottleWidget*> bottleWidgets;
    int capacity;
};
#endif // MAINWINDOW_H
