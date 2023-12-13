#ifndef BATTERY_H
#define BATTERY_H

#include <QPushButton>
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
    int getBatteryLevel() const { return batteryLevel; }

signals:
    void batteryLevelChanged(int level);

public slots:
    void start();
    void stop();
    void updateBatteryLevel();
    void chargeBattery();
    void toggleCharging();

private:
    QLCDNumber* batteryLCD;
    QPushButton* chargeButton;
    QTimer* batteryTimer;
    QTimer* chargingTimer;
    int batteryLevel;
    bool isCharging;
};

#endif // BATTERY_H
