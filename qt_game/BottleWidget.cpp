#include "BottleWidget.h"
#include <QPainter>

#include<QPainterPath>

BottleWidget::BottleWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumSize(70,220);
}

void BottleWidget::setBottle(const std::vector<int>& bottle)
{
    water = bottle;

    update(); // 触发重绘
}
void BottleWidget::setCapacity(int c)
{
    capacity = c;
}

void BottleWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();

    QRect bottleRect(10,10,w-20,h-20);

    // ===== 画玻璃瓶 =====

    painter.setPen(QPen(QColor(60,60,60),3));
    painter.setBrush(Qt::NoBrush);

    painter.drawRoundedRect(bottleRect,15,15);

    // ===== 计算水层高度 =====



    int waterHeight = (h-40)/capacity;

    for(int i=0;i<water.size();i++)
    {
        QColor c;

        switch(water[i])
        {
        case 1: c = QColor(255, 80, 80); break;    // 鲜红色
        case 2: c = QColor(80, 120, 255); break;   // 亮蓝色
        case 3: c = QColor(80, 200, 120); break;   // 翠绿色
        case 4: c = QColor(255, 210, 80); break;   // 金黄色
        case 5: c = QColor(200, 100, 255); break;  // 紫罗兰
        case 6: c = QColor(255, 150, 200); break;  // 粉红色
        case 7: c = QColor(100, 200, 255); break;  // 天蓝色
        case 8: c = QColor(255, 160, 80); break;   // 橙色
        case 9: c = QColor(160, 255, 160); break;  // 浅绿色
        case 10: c = QColor(210, 180, 140); break; // 卡其色/棕色

        default: c = QColor(150, 150, 150);         // 灰色
        }

        int y = h-20-(i+1)*waterHeight;

        QRect layer(12,y,w-24,waterHeight);

        painter.setPen(Qt::NoPen);
        painter.setBrush(c);

        // 圆角水面
        QPainterPath path;

        path.addRoundedRect(layer,6,6);

        painter.drawPath(path);
    }

    if(water.size()==capacity)
    {
        bool same=true;

        for(int i=1;i<water.size();i++)
        {
            if(water[i]!=water[0])
                same=false;
        }

        if(same)
        {

            painter.setPen(QPen(Qt::white,4));

            painter.drawText(rect(),Qt::AlignCenter,"✔");
        }
    }

    // ===== 画高光（玻璃效果）=====

    painter.setPen(Qt::NoPen);

    QColor highlight(255,255,255,40);

    painter.setBrush(highlight);

    painter.drawRoundedRect(15,15,8,h-30,6,6);
}

void BottleWidget::mousePressEvent(QMouseEvent *)
{
    emit clicked(this);
}

