#include "BottleWidget.h"
#include <QPainter>

BottleWidget::BottleWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(60,200);
}

void BottleWidget::setBottle(const std::vector<int>& bottle)
{
    water = bottle;

    update(); // 触发重绘
}

void BottleWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    // 画瓶子边框
    painter.setPen(QPen(Qt::black,3));

    painter.drawRoundedRect(10,10,w-20,h-20,10,10);

    int layerHeight = (h-40)/4;

    for(int i=0;i<water.size();i++)
    {
        int color = water[i];

        QColor c;

        switch(color)
        {
        case 1: c = Qt::red; break;
        case 2: c = Qt::blue; break;
        case 3: c = Qt::green; break;
        case 4: c = Qt::yellow; break;
        default: c = Qt::gray;
        }

        painter.setBrush(c);

        int y = h-20-(i+1)*layerHeight;

        painter.drawRect(12,y,w-24,layerHeight);
    }
}
