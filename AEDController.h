#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H
#include "./ui_mainwindow.h"

#include "HeartRateMonitor.h"
#include "AEDPlacement.h"

#include <QObject>
#include <QTextBrowser>

class AEDController : public QObject
{
    Q_OBJECT

public:
    enum AEDState
    {
        PhysicalError, // if the  AED is not on the body anymore...
        PowerOff,
        StandClear,
        Analyzing,
        ShockAdvised,
        Shock,
        PostShockCare, // could be a transition
        ContinuedEvaluation // this can be the default if nothing is wrong
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

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

private slots:
    void update();
    void resetHeartbeat();
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;
    AEDState state;

    QTextBrowser* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;

    QTimer* updateTimer;
    QTimer* restartHeartbeat;
};

#endif
