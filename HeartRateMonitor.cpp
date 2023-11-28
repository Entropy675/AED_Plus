#include "HeartRateMonitor.h"

// an object that displays the heart rate on the screen.

HeartRateMonitor::HeartRateMonitor(QWidget *parent, int startHeartRate, int width, int height)
    : QGraphicsScene(parent), vWidth(width - 10), vHeight(height - 10)
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
        items->setX(items->x() - 3);
        if(items->x() < -vWidth)
        {
            this->removeItem(items);
        }
    }


    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(1, 1, 8, 8); // Adjust the rectangle as needed

    tempCounter += 4;
    pointItem->setBrush(QColor((tempCounter % 255), 111, 111)); // Set the color of the point
    pointItem->setPen(Qt::NoPen); // Set the pen (outline) to be transparent
    pointItem->setPos(0, -tempCounter);


    qDebug() << " Bounding rect: "<< this->itemsBoundingRect();
    if(tempCounter > 251)
        tempCounter = 0;

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
