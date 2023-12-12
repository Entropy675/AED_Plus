#ifndef BATTERY_H
#define BATTERY_H

#include <QGraphicsScene>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QRandomGenerator>

class Battery : public QGraphicsScene
{
    Q_OBJECT

public:
    Battery(QWidget* lcdParent = nullptr);
    ~Battery();

signals:
    void batteryLevelChanged(int level);

public slots:
    void start();
    void stop();
    void updateBatteryLevel();

private:
    QLCDNumber* batteryLCD;
    QTimer* batteryTimer;
    int batteryLevel;
};

#endif // BATTERY_H
