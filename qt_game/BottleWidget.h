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
    void mousePressEvent(QMouseEvent*)override;

private:
    std::vector<int> water;
    int capacity;
signals:
    void clicked(BottleWidget*);



};

#endif // BOTTLEWIDGET_H
