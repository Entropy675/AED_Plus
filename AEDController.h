#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H
#include "./ui_mainwindow.h"
#include "Battery.h"
#include "HeartRateMonitor.h"
#include "AEDPlacement.h"
#include "aedring.h"


#include <QObject>
#include <QTextBrowser>

class AEDController : public QObject
{
    Q_OBJECT

public:

    AEDController(Ui::MainWindow& ui);
    ~AEDController();

    void powerOn();
    void powerOff();

signals:
    // add signals here

public slots:
    // add slots that recieve signals here
    void appendToDisplay(QString);
    void electrocutePressed();

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

    void updateAEDRingState();

private slots:
    void resetHeartbeat();
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;
    AEDState currState;

    QTextBrowser* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;

    AEDRing* aedRing;

    Battery* battery;
    bool isPowerDown;
    void powerDown();
    void enableAllComponents();
    void disableAllComponents();

    void enableAEDPlacement();
    QPushButton* powerButton;

    QTimer* updateTimer;
    QTimer* restartHeartbeat;
};

#endif
