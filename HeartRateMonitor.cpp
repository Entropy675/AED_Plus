#include "HeartRateMonitor.h"
#include <cmath>

// an object that displays the heart rate on the screen.

HeartRateMonitor::HeartRateMonitor(QWidget *parent, int width, int height)
    : QGraphicsScene(parent), vWidth(width - 10), vHeight(height - 10)
{
    heartRateTimer = new QTimer(this);
    connect(heartRateTimer, &QTimer::timeout, this, &HeartRateMonitor::heartBeat);

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

void HeartRateMonitor::startAnalyzing(double heartRate)
{
    // heartRateTimer->stop();
    heartRateTimer->start(1000/(heartRate/60.0)*HEART_RATE_SCALE); // scaled down by 1.5 for fitting it all in a small space
    // qDebug() << 1000/(startHeartRate/60.0) << " " << startHeartRate;
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


    QGraphicsEllipseItem* pointItemPre = new QGraphicsEllipseItem(1, 1, 3, 4); // Adjust the rectangle as needed
    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(1, 1, 3, 5); // Adjust the rectangle as needed
    QGraphicsEllipseItem* pointItemPost = new QGraphicsEllipseItem(1, 1, 3, 6); // Adjust the rectangle as needed
    if(redColorShift > MINIMUM_RED_COLOR) // max
        redColorShift -= 4;

    if(heartBeatOccurring)
    {
        if(HEART_RATE_MON_LOG)
            qDebug() << -50*heartBeatFunc(-heartBeatOccurring) << " " << heartBeatOccurring;
        pointItemPre->setPos(0, -50*heartBeatFunc(-heartBeatOccurring + 0.007));
        pointItem->setPos(0, -50*heartBeatFunc(-heartBeatOccurring)); // -50 is scale, its negative because think of what we are drawing of as a reflection...
        pointItemPost->setPos(0, -50*heartBeatFunc(-heartBeatOccurring - 0.007));
        heartBeatOccurring -= 0.022;
        if(heartBeatOccurring < 0)
            heartBeatOccurring = 0;
    }

    pointItemPre->setBrush(QColor(redColorShift, 111, 111)); // Set the color of the point
    pointItemPre->setPen(Qt::NoPen); // Set the pen (outline) to be transparent
    pointItem->setBrush(QColor(redColorShift, 111, 111)); // Set the color of the point
    pointItem->setPen(Qt::NoPen); // Set the pen (outline) to be transparent
    pointItemPost->setBrush(QColor(redColorShift, 111, 111)); // Set the color of the point
    pointItemPost->setPen(Qt::NoPen); // Set the pen (outline) to be transparent

    this->addItem(pointItemPre);
    this->addItem(pointItem);
    this->addItem(pointItemPost);

    if(((int)(heartBeatOccurring*100) % 10) == 0 && heartBeatOccurring)
    {
        QGraphicsEllipseItem* pointItem2 = new QGraphicsEllipseItem(1, 1, 3, 4); // Adjust the rectangle as needed
        pointItem2->setBrush(QColor(110, 145, 145)); // Set the color of the point
        pointItem2->setPen(Qt::NoPen); // Set the pen (outline) to be transparent
        this->addItem(pointItem2);
    }
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFunc(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    if(HEART_RATE_MON_LOG)
        emit pushTextToDisplay(QString::number(std::sin(x * 3.14 * 10 + 5.759) + 0.5) + " ");
    return std::sin(x * 3.14 * 10 + 5.759) + 0.5;
}

// runs every beat
void HeartRateMonitor::heartBeat()
{
    heartBeatOccurring = 1; // set to 1 to start the heart beat
    redColorShift = 255;
}

void HeartRateMonitor::updateHeartRate(int newHeartRateBPM)
{
    heartRateTimer->stop();
    if(newHeartRateBPM)
        heartRateTimer->start(1000/(newHeartRateBPM/60.0)*HEART_RATE_SCALE); // 60 seconds in a minute, 1000ms ;) (scaled it by 1.5 just for display purposes)
}
