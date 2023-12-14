#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H

#include <QObject>

#include "./ui_mainwindow.h"

#include "Battery.h"
#include "HeartRateMonitor.h"
#include "OutputTextbox.h"
#include "AEDPlacement.h"
#include "AEDRing.h"
#include "AEDCompressionBar.h"
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
    void updateCPRButtonClicked();
    void updateCPRTimer();
private slots:
    void heartRhythmChanged(int index);
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;

    OutputTextbox* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;

    AEDRing* aedRing;

    AEDCompressionBar *cprBar;

    Battery* battery;
    bool isPowerOn;
    void batterydead();
    void enableAllComponents();
    void disableAllComponents();

    void turnPowerButtonOn();
    void turnPowerButtonOff();

    QPixmap powerButtonImageOn;
    QPixmap powerButtonImageOff;

    QTimer* restartHeartbeat;

    QTimer *cprTimer;
    int cprTimerInterval;
    int cprTimeRemaining;

    int cprIndex = 1;
};

#endif
