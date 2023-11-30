#include "HeartRateMonitor.h"
#include <cmath>

// an object that displays the heart rate on the screen.

HeartRateMonitor::HeartRateMonitor(QWidget *parent, double startHeartRate, int width, int height)
    : QGraphicsScene(parent), vWidth(width - 10), vHeight(height - 10)
{
    heartRateTimer = new QTimer(this);
    connect(heartRateTimer, &QTimer::timeout, this, &HeartRateMonitor::heartBeat);
    heartRateTimer->start(1000/(startHeartRate/60.0));
    // qDebug() << 1000/(startHeartRate/60.0) << " " << startHeartRate;

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &HeartRateMonitor::updatePosition);
    updateTimer->start(PING_RATE_MS);
}

HeartRateMonitor::~HeartRateMonitor()
{
    heartRateTimer->stop();
    delete heartRateTimer;
    heartRateTimer = nullptr;

    updateTimer->stop();
    delete updateTimer;
    updateTimer = nullptr;
}


// runs at PING_RATE_MS
void HeartRateMonitor::updatePosition()
{
    // moves all points to the left (decrement x), deletes offscreen one
    for(QGraphicsItem* items : this->items())
    {
        items->setX(items->x() - 1);
        if(items->x() < -vWidth)
            this->removeItem(items);
    }


    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(1, 1, 3, 8); // Adjust the rectangle as needed
    if(redColorShift > 30)
        redColorShift -= 4;

    if(heartBeatOccurring)
    {
        // qDebug() << -50*heartBeatFunc(-heartBeatOccurring) << " " << heartBeatOccurring;
        pointItem->setPos(0, -50*heartBeatFunc(-heartBeatOccurring));
        heartBeatOccurring -= 0.022;
        if(heartBeatOccurring < 0)
            heartBeatOccurring = 0;
    }

    pointItem->setBrush(QColor(redColorShift, 111, 111)); // Set the color of the point
    pointItem->setPen(Qt::NoPen); // Set the pen (outline) to be transparent

    this->addItem(pointItem);
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFunc(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    emit pushTextToDisplay(QString::number(std::sin(x * 3.14 * 10 + 5.759) + 0.5) + " ");
    return std::sin(x * 3.14 * 10 + 5.759) + 0.5;
}

// runs every beat
void HeartRateMonitor::heartBeat()
{
    heartBeatOccurring = 1;
    redColorShift = 255;
}

void HeartRateMonitor::updateHeartRate(int newHeartRateBPM)
{
    heartRateTimer->stop();
    heartRateTimer->start(1000/(newHeartRateBPM/60.0)); // 60 seconds in a minute, 1000ms ;)
}
