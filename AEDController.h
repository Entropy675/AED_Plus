#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H
#include "./ui_mainwindow.h"
#include "Battery.h"
#include "HeartRateMonitor.h"
#include "OutputTextbox.h"
#include "AEDPlacement.h"

#include <QObject>

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

signals:
    // add signals here

public slots:
    // add slots that recieve signals here
    void appendToDisplay(QString);
    void electrocutePressed();
    void powerDown();

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

private slots:
    void update();
    void AEDAttachedStartAnalyzing();

private:
    const Ui::MainWindow& ui;
    AEDState state;

    OutputTextbox* outputText;
    HeartRateMonitor* hMonitor;
    AEDPlacement* aedPlacementDemo;
    Battery* battery;
    bool isPowerDown;
    void batterydead();
    void enableAllComponents();
    void disableAllComponents();
    QTimer* updateTimer;
    QPushButton* powerButton;
};

#endif
