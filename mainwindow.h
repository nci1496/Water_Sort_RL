#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <vector>

class BottleWidget;

class MainWindow : public QWidget
{
    Q_OBJECT

public:

    MainWindow(QWidget* parent=nullptr);

    void updateGame(const std::vector<std::vector<int>>& bottles);

private:

    std::vector<BottleWidget*> bottleWidgets;

};
#endif // MAINWINDOW_H
