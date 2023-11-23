#ifndef AEDCONTROLLER_H
#define AEDCONTROLLER_H
#include <QObject>
#include "./ui_mainwindow.h"


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
        PostShockCare,
        ContinuedEvaluation // this can be the default if nothing is wrong
    }; // we can add more/remove some as we need

    AEDController(Ui::MainWindow& ui);

signals:
    // add signals this sends across all threads here
    // (Likely condition variable encapsulation - its just semaphores...)

public slots:
    // add slots that recieve signals here

    // for rescaling the layout based on new screen size...
    void handleScreenResized(int w, int h);

private:
    const Ui::MainWindow& ui;
};

#endif
