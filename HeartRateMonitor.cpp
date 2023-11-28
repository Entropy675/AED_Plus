#include "HeartRateMonitor.h"
#include <cmath>

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
        items->setX(items->x() - 1);
        if(items->x() < -vWidth)
        {
            this->removeItem(items);
        }
    }


    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(1, 1, 3, 8); // Adjust the rectangle as needed
    if(loading > 30)
        loading -= 8;

    if(heartBeatOccurring)
    {
        qDebug() << -50*heartBeatFunc(-heartBeatOccurring) << " " << heartBeatOccurring;
        pointItem->setPos(0, -50*heartBeatFunc(-heartBeatOccurring));
        heartBeatOccurring -= 0.02;
        if(heartBeatOccurring < 0)
            heartBeatOccurring = 0;
    }

    pointItem->setBrush(QColor(loading, 111, 111)); // Set the color of the point
    pointItem->setPen(Qt::NoPen); // Set the pen (outline) to be transparent

    this->addItem(pointItem);


    /* some messing around code
    pointItem->setBrush(QColor((tempCounter % 255), 111, 111)); // Set the color of the point
    tempCounter += 4;
    if(tempCounter > 100)
        pointItem->setPos(0, tempCounter);
    else
        pointItem->setPos(0, 100);

    qDebug() << " Bounding rect: "<< this->itemsBoundingRect();
    if(tempCounter > 251)
        tempCounter = 0;
    */

}

// we can use the function sin(2*x*pi) + 0.4 for range 0 to 0.935 to simulate a bump
// sin(10*pi*x + 5.75) + 0.5 is better...
// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFunc(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    return std::sin(x * 3.14 * 10 + 5.759) + 0.5;
}

// runs every beat
void HeartRateMonitor::heartBeat()
{
    heartBeatOccurring = 1;
    loading = 255;
}

void HeartRateMonitor::updateHeartRate(int newHeartRateBPM)
{
    heartRateTimer->stop();
    heartRateTimer->start(newHeartRateBPM/60 * 1000); // 60 seconds in a minute
}
