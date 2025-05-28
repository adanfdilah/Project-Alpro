#include "equalizerwidget.h"
#include <QPainter>
#include <QRandomGenerator>

EqualizerWidget::EqualizerWidget(QWidget *parent)
    : QWidget(parent), barHeights(10, 0)
{
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &EqualizerWidget::updateBars);
    animationTimer->start(250); // update tiap 100ms
}

void EqualizerWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::green);
    painter.setPen(Qt::NoPen);

    int barWidth = width() / barHeights.size();
    for (int i = 0; i < barHeights.size(); ++i) {
        int barHeight = barHeights[i];
        painter.drawRect(i * barWidth, height() - barHeight, barWidth - 2, barHeight);
    }
}

void EqualizerWidget::updateBars()
{
    for (int &height : barHeights) {
        height = QRandomGenerator::global()->bounded(height() / 2, height()); // acak
    }
    update(); // repaint
}
