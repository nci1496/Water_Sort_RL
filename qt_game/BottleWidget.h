#ifndef BOTTLEWIDGET_H
#define BOTTLEWIDGET_H

#include <QWidget>
#include<vector>

class BottleWidget : public QWidget
{
    Q_OBJECT
public:
    BottleWidget(QWidget *parent = nullptr);
    void setBottle(const std::vector<int>&bottle);
    void setCapacity(int c);

protected:
    void paintEvent(QPaintEvent* event)override;

private:
    std::vector<int> water;
    int capacity;
signals:
};

#endif // BOTTLEWIDGET_H
