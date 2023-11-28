#include "HeartRateMonitor.h"

// an object that displays the heart rate on the screen.

HeartRateMonitor::HeartRateMonitor(QWidget *parent, int startHeartRate)
    : QGraphicsScene(parent)
{
    heartRateTimer = new QTimer(this);
    connect(heartRateTimer, &QTimer::timeout, this, &HeartRateMonitor::heartBeat);
    heartRateTimer->start(startHeartRate/60 * 1000); // 60 seconds in a minute

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &HeartRateMonitor::updatePosition);
    updateTimer->start(PING_RATE_MS); // 60 seconds in a minute
}

// runs at PING_RATE_MS
// moves all points to the left (increment virtual x)

void HeartRateMonitor::updatePosition()
{
    for(QGraphicsItem* items : this->items())
    {
        items->setX(items->x() - 6);
        if(items->x() < -140)
        {
            QGraphicsEllipseItem* t = (QGraphicsEllipseItem*)items;
            t->setBrush(Qt::green);
        }

        if(items->x() < -160)
        {
            this->removeItem(items);
        }
    }

    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(2, 2, 10, 10); // Adjust the rectangle as needed
    QColor color((8*tempCounter++ % 254), 111, 111);
    pointItem->setBrush(color); // Set the color of the point
    pointItem->setPos(2, 2);

    this->addItem(pointItem);
}

// runs every beat
void HeartRateMonitor::heartBeat()
{

}

void HeartRateMonitor::updateHeartRate(int newHeartRateBPM)
{
    heartRateTimer->stop();
    heartRateTimer->start(newHeartRateBPM/60 * 1000); // 60 seconds in a minute
}
