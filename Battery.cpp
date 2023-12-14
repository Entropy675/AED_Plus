#include "Battery.h"

Battery::Battery(QWidget* lcdParent)
    : QGraphicsScene(lcdParent), batteryLevel(100)
{
    batteryLCD = new QLCDNumber(lcdParent);
    batteryLCD->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    batteryLCD->setFixedSize(100, 50);

    chargeButton = new QPushButton("Charge", lcdParent);
    connect(chargeButton, &QPushButton::clicked, this, &Battery::toggleCharging);
    chargeButton->setGeometry(0, 50, 100, 30);

    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, &Battery::updateBatteryLevel);

    chargingTimer = new QTimer(this);
    connect(chargingTimer, &QTimer::timeout, this, &Battery::chargeBattery);
}

Battery::~Battery()
{
}

void Battery::start()
{
    // Seed the random number generator with the current time
    QRandomGenerator(QTime::currentTime().msec());

    // Generate a random number between 1000 and 5000 milliseconds
    int randomInterval = QRandomGenerator::global()->bounded(1000, 5000);

    batteryTimer->start(randomInterval); // Set the interval as a random value
}

void Battery::stop()
{
    batteryTimer->stop();
}

void Battery::updateBatteryLevel()
{
    batteryLevel = qMax(batteryLevel - 1, 0);
    batteryLCD->display(batteryLevel);

    emit batteryLevelChanged(batteryLevel);

    if (batteryLevel == 0) {
        stop();
    }else if (batteryLevel == 20){
        qDebug() << "Battery Level is " << batteryLevel <<"% " << "please charge or replace the battery";
    }

}

void Battery::chargeBattery()
{
    if (batteryLevel < 100) {
        batteryTimer->stop();
        batteryLevel = qMin(batteryLevel + 2, 100);
        batteryLCD->display(batteryLevel);
        emit batteryLevelChanged(batteryLevel);

        if (!chargingTimer->isActive()) {
            chargingTimer->start(2000);
        }
    } else {
        chargingTimer->stop();
        batteryTimer->start(QRandomGenerator::global()->bounded(1000, 5000));
    }
}

void Battery::toggleCharging()
{
    isCharging = !isCharging;

    if (isCharging) {
        chargingTimer->start(2000);
        batteryTimer->stop();
        chargeButton->setText("Charging");
    } else {
        chargingTimer->stop();
        batteryTimer->start();
        chargeButton->setText("Charge");
    }
}
