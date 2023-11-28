#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include <QObject>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

// might want to extend some other kind of displayable object?
class HeartRateMonitor : public QGraphicsScene
{

public:
    HeartRateMonitor(QWidget *parent = nullptr);

public slots:
    void updatePosition();

private:
    // we can use the function sin(2*x*pi) + 0.4 for range 0 to 0.935 to simulate a bump

};

#endif // HEARTRATEMONITOR_H
