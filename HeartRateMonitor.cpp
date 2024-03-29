#include "HeartRateMonitor.h"
#include <cmath>

// an object that displays the heart rate on the screen.


HeartRateMonitor::HeartRateMonitor(QWidget *parent, QLCDNumber* lcd, int width, int height)
    : QGraphicsScene(parent), vWidth(width - 10), vHeight(height - 10), bpmLCD(lcd)
{
    heartRateTimer = new QTimer(this);
    connect(heartRateTimer, &QTimer::timeout, this, &HeartRateMonitor::heartBeat);
    powerOn();

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &HeartRateMonitor::updatePosition);
    updateTimer->start(PING_RATE_MS);

    std::srand(static_cast<unsigned int>(std::time(0)));
    powerOff();
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

bool HeartRateMonitor::isOn()
{
    return power;
}

void HeartRateMonitor::powerOn()
{
    power = true;
}

void HeartRateMonitor::powerOff()
{
    power = false;
    heartRateTimer->stop();
}

void HeartRateMonitor::startAnalyzing(int heartRate)

{
    // heartRateTimer->stop();
    heartRateTimer->start(1000/(heartRate/60.0)*HEART_RATE_SCALE); // scaled down by 1.5 for fitting it all in a small space
    // qDebug() << 1000/(startHeartRate/60.0) << " " << startHeartRate;
}

void HeartRateMonitor::changeRhythm(HeartBeatType type)
{
    rhythm = type;
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

    if(!power)
        return;

    QGraphicsEllipseItem* pointItemPre = new QGraphicsEllipseItem(1, 1, 3, 4); // Adjust the rectangle as needed
    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem(1, 1, 3, 5); // Adjust the rectangle as needed
    QGraphicsEllipseItem* pointItemPost = new QGraphicsEllipseItem(1, 1, 3, 6); // Adjust the rectangle as needed
    if(redColorShift > MINIMUM_RED_COLOR) // max
        redColorShift -= 4;

    if(heartBeatOccurring)
    {
        switch(rhythm)
        {
        case PEA:
            pointItemPre->setPos(0, -50*heartBeatFuncPEA(-heartBeatOccurring + 0.007));
            pointItem->setPos(0, -50*heartBeatFuncPEA(-heartBeatOccurring)); // -50 is scale, its negative because think of what we are drawing of as a reflection...
            pointItemPost->setPos(0, -50*heartBeatFuncPEA(-heartBeatOccurring - 0.007));
            break;
        case ASYSTOLE:

            pointItemPre->setPos(0, -50*heartBeatFuncASYS(-heartBeatOccurring + 0.007));
            pointItem->setPos(0, -50*heartBeatFuncASYS(-heartBeatOccurring)); // -50 is scale, its negative because think of what we are drawing of as a reflection...
            pointItemPost->setPos(0, -50*heartBeatFuncASYS(-heartBeatOccurring - 0.007));
            break;
        case VF:

            pointItemPre->setPos(0, -50*heartBeatFuncVF(-heartBeatOccurring + 0.007));
            pointItem->setPos(0, -50*heartBeatFuncVF(-heartBeatOccurring)); // -50 is scale, its negative because think of what we are drawing of as a reflection...
            pointItemPost->setPos(0, -50*heartBeatFuncVF(-heartBeatOccurring - 0.007));
            break;
        case VT:

            pointItemPre->setPos(0, -50*heartBeatFuncVT(-heartBeatOccurring + 0.007));
            pointItem->setPos(0, -50*heartBeatFuncVT(-heartBeatOccurring)); // -50 is scale, its negative because think of what we are drawing of as a reflection...
            pointItemPost->setPos(0, -50*heartBeatFuncVT(-heartBeatOccurring - 0.007));
            break;
        }


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

        switch(rhythm)
        {
        case PEA:
            bpmLCD->display(bpm + (std::rand() % (bpmVariation*2+1) - bpmVariation));
            break;
        case ASYSTOLE:
            bpmLCD->display(0);
            break;
        case VF:
             bpmLCD->display(bpm + (std::rand() % (bpmVariation*2+1) - bpmVariation) + 80);
            break;
        case VT:
            bpmLCD->display(bpm + (std::rand() % (bpmVariation*2+1) - bpmVariation) + 95);
            break;
        }
        QGraphicsEllipseItem* pointItem2 = new QGraphicsEllipseItem(1, 1, 3, 4); // Adjust the rectangle as needed
        pointItem2->setBrush(QColor(110, 145, 145)); // Set the color of the point
        pointItem2->setPen(Qt::NoPen); // Set the pen (outline) to be transparent
        this->addItem(pointItem2);
    }

    qDebug() << heartBeatOccurring;
    if(heartBeatOccurring < 0.01 && (rhythm != PEA) && (rhythm != PEA))
    {
        qDebug() << heartBeatOccurring << " rhythm: " << rhythm;
        if(rhythm == 2)
            heartBeatOccurring = 1000;
        else
            heartBeatOccurring = 1;
    }
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFuncPEA(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    if(HEART_RATE_MON_LOG)
        emit pushTextToDisplay(QString::number(std::sin(x * 3.14 * 10 + 5.759) + 0.5) + " ");
    return std::sin(x * 3.14 * 10 + 5.759) + 0.5;
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFuncASYS(double x)
{
    using namespace std;
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    double out = 0.6 * (
                     (1.0/24.0) * sin(x * 3.14 * 10 + 2) +
                     0.6 +
                     (1.0/24.0) * sin(x * 20 + 5.8) +
                     (1.0/24.0) * cos(x * 27) +
                     (1.0/65.0) * cos(x * 121 - 15));

    if(HEART_RATE_MON_LOG)
        emit pushTextToDisplay(QString::number(out) + " ");
    return out/2;
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFuncVF(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    double out = (1.0/7.0) * sin(x * 3.14 * 10 + 2) +
                 0.6 +
                 (1.0/12.0) * sin(x * 20 + 5.8) +
                 (1.0/12.0) * cos(x * 27) +
                 (1.0/20.0) * cos(x * 121 - 15);

    if(HEART_RATE_MON_LOG)
        emit pushTextToDisplay(QString::number(out) + " ");
    return out;
}

// takes in a value from 0 to 1
// outputs values between 1.5 and -0.5
double HeartRateMonitor::heartBeatFuncVT(double x)
{
    x /= 5; // looks closer to heartbeat between x = 0 and 0.2
    if(HEART_RATE_MON_LOG)
        emit pushTextToDisplay(QString::number((1.0/5.0) * sin(30 * x) + 0.6 + (1.0/15.0) * sin(1.1 * x)) + " ");
    return (1.0/5.0) * sin(30 * x) + 0.6 + (1.0/15.0) * sin(1.1 * x);
}


// runs every beat
void HeartRateMonitor::heartBeat()
{
    if(rhythm == PEA)
        heartBeatOccurring = 1; // set to 1 to start the heart beat
    redColorShift = 255;

}

void HeartRateMonitor::updateHeartRate(int newHeartRateBPM)
{
    heartRateTimer->stop();
    if(newHeartRateBPM)
        heartRateTimer->start(1000/(newHeartRateBPM/60.0)*HEART_RATE_SCALE); // 60 seconds in a minute, 1000ms ;) (scaled it by 1.5 just for display purposes)

    int newRandomBpm = bpmLCD->intValue() + (std::rand() % (bpmVariation*2+1) - bpmVariation);

    // emit pushTextToDisplay(QString("New BPM: %1").arg(newRandomBpm));

    if(rhythm == PEA)
        heartRateTimer->start(1000/(newRandomBpm/60.0)*HEART_RATE_SCALE);

}
