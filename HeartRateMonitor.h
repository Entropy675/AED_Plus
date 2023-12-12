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
    Q_OBJECT

public:
    HeartRateMonitor(QWidget *parent = nullptr, int width = 200, int height = 200);
    ~HeartRateMonitor();

    void updateHeartRate(int newHeartRateBPM);
    void startAnalyzing(int heartBPM = START_HEART_RATE);

signals:
    void pushTextToDisplay(QString s);

private slots:
    void updatePosition();
    void heartBeat();

private:
    int redColorShift = 255;
    int vWidth, vHeight; // views width and height
    double heartBeatOccurring = 0;
    int bpm = 0;
    int bpmVariation = HEART_BPM_VARIATION; // increase this by a lot dynamically to simulate arrhythmia

    // we can use the function sin(2*x*pi) + 0.4 for range 0 to 0.935 to simulate a bump
    // maybe sin(10*pi*x + 5.75) + 0.5 is better... looks closer to heartbeat between 0 and 0.2
    double heartBeatFunc(double x);
    QTimer* updateTimer;
    QTimer* heartRateTimer;
};

#endif // HEARTRATEMONITOR_H
