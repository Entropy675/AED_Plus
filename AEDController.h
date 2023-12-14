#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H

#include <QObject>

#include "./ui_mainwindow.h"

#include "Battery.h"
#include "HeartRateMonitor.h"
#include "OutputTextbox.h"
#include "AEDPlacement.h"
#include "AEDRing.h"

class AEDController : public QObject
{
    Q_OBJECT

public:
    AEDController(Ui::MainWindow& ui);
    ~AEDController();

    void powerOn();
    void powerOff();

public slots:
    // add slots that recieve signals here
    void appendToDisplay(QString);
    void electrocutePressed();
    void power();

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

    void updateAEDRingState();

private slots:
    void resetHeartbeat();
    void heartRhythmChanged(int index);
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;

    OutputTextbox* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;

    AEDRing* aedRing;

    Battery* battery;
    bool isPowerOn;
    void batterydead();
    void enableAllComponents();
    void disableAllComponents();

    void turnPowerButtonOn();
    void turnPowerButtonOff();

    QPushButton* powerButton;

    QPixmap powerButtonImageOn;
    QPixmap powerButtonImageOff;

    QTimer* restartHeartbeat;
};

#endif
