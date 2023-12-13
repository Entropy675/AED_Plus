#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H
#include "./ui_mainwindow.h"
#include "Battery.h"
#include "HeartRateMonitor.h"
#include "OutputTextbox.h"
#include "AEDPlacement.h"
#include "aedring.h"


#include <QObject>

class AEDController : public QObject
{
    Q_OBJECT

public:

  enum AEDState
    {
    Default,
    AnalyzingResponsiveness,
    EmergencyServices,
    Breathing, // use lid to put behind victims shoulders to maintain an effective airway (dont use support if spinal injury)
    ElectrodePlacement,
    Shock, // shock is also HeartRythmAnalysis,this is where it will be judged
    PostShockCare, // bunched with continued evaluation
    }; // we can add more/remove some as we need

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
    void powerDown();

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

    void updateAEDRingState();

private slots:
    void resetHeartbeat();
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;
    AEDState currState;

    OutputTextbox* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;

    AEDRing* aedRing;

    Battery* battery;
    bool isPowerDown;
    void batterydead();
    void powerDown();
    void enableAllComponents();
    void disableAllComponents();

    void enableAEDPlacement();
    QPushButton* powerButton;

    QTimer* updateTimer;
    QPushButton* powerButton;
};

#endif
