#include "Battery.h"

Battery::Battery(QWidget* lcdParent)
    : QGraphicsScene(lcdParent), batteryLevel(100)
{
    batteryLCD = new QLCDNumber(lcdParent);
    batteryLCD->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    batteryLCD->setFixedSize(100, 50);
    batteryTimer = new QTimer(this);
    connect(batteryTimer, &QTimer::timeout, this, &Battery::updateBatteryLevel);

}

Battery::~Battery()
{
}

void Battery::start()
{
    // Seed the random number generator with the current time
    QRandomGenerator(QTime::currentTime().msec());

    // Generate a random number between 1000 and 3000 milliseconds (adjust the range as needed)
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
    }
}
