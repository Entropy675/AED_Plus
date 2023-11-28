#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include "defs.h"

#include <QObject>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>

// might want to extend some other kind of displayable object?
class HeartRateMonitor : public QGraphicsScene
{

public:
    HeartRateMonitor(QWidget *parent = nullptr, int heartRateSec = START_HEART_RATE, int width = 200, int height = 200);

    void updateHeartRate(int newHeartRateBPM);

private slots:
    void updatePosition();
    void heartBeat();

private:
    // we can use the function sin(2*x*pi) + 0.4 for range 0 to 0.935 to simulate a bump
    int tempCounter = 0;
    int vWidth, vHeight; // views with and height

    QTimer* updateTimer;
    QTimer* heartRateTimer;
};

#endif // HEARTRATEMONITOR_H
