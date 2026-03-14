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
    void setSelected(bool s);

protected:
    void paintEvent(QPaintEvent* event)override;
    void mousePressEvent(QMouseEvent*)override;
    void showEvent(QShowEvent* event)override;

private:
    std::vector<int> water;
    int capacity;
    bool selected=false;
    QPoint basePos;//用于选中瓶子上移

signals:
    void clicked(BottleWidget*);



};

#endif // BOTTLEWIDGET_H
