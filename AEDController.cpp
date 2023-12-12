#include "AEDController.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QThread>

AEDController::AEDController(Ui::MainWindow& u)
    : ui(u), currState(Default)
{
    hMonitor = new HeartRateMonitor(nullptr, u.HeartRateView->width(), u.HeartRateView->height());
    u.HeartRateView->setScene(hMonitor);

    // connect signal from HeartRateMonitor to this classes slot
    connect(hMonitor, &HeartRateMonitor::pushTextToDisplay, this, &AEDController::appendToDisplay);

    updateTimer = new QTimer(this);
    //connect(updateTimer, &QTimer::timeout, this, &AEDController::update); ????? why use update
    updateTimer->start(PING_RATE_MS);

    restartHeartbeat = new QTimer(this);
    restartHeartbeat->setSingleShot(true);
    connect(restartHeartbeat, &QTimer::timeout, this, &AEDController::resetHeartbeat);

    outputText = new QTextBrowser(ui.outputTextGroupBox);

    QVBoxLayout* outputBoxLayout = new QVBoxLayout();
    outputBoxLayout->addWidget(outputText);
    ui.outputTextGroupBox->setLayout(outputBoxLayout);

    aedRing = new AEDRing(ui.AEDRingView);
    connect(aedRing, &AEDRing::updateAEDState, this, &AEDController::updateAEDRingState);

    battery = new Battery(u.BatteryView);

    connect(ui.pushButton1, &QPushButton::clicked, this, &AEDController::powerDown);

    battery->start();


}

AEDController::~AEDController()
{
    updateTimer->stop();
    delete updateTimer;

    delete hMonitor;
    delete outputText;
    delete aedPlacementDemo;
}

void AEDController::handleScreenResized(int w, int h)
{
    // we could rescale the UI element sizes here...
    // new window widths are w and h
}

void AEDController::appendToDisplay(QString s)
{
    outputText->append(s);
}

void AEDController::AEDAttachedStartAnalyzing()
{
    hMonitor->startAnalyzing();
}

void AEDController::electrocutePressed()
{
    if(currState != Shock) {
        appendToDisplay("Shockable rhythm not yet detected. ");
        return;
    }
    else {
        appendToDisplay("Electricution delivered!");
        qDebug("Shock is delivered to the patient!!!!");
        hMonitor->updateHeartRate(300);
        restartHeartbeat->start(1200);
    }
}

void AEDController::enableAEDPlacement(){
    aedPlacementDemo = new AEDPlacement(ui.patientBodyBox);
    connect(aedPlacementDemo, &AEDPlacement::pushTextToDisplay, this, &AEDController::appendToDisplay);
    connect(aedPlacementDemo, &AEDPlacement::AEDAttachedToPatient, this, &AEDController::AEDAttachedStartAnalyzing);
    connect(aedPlacementDemo, &AEDPlacement::electrocutePatientPressed, this, &AEDController::electrocutePressed);
}

void AEDController::resetHeartbeat()
{

    hMonitor->updateHeartRate(START_HEART_RATE);
    qDebug("Patient stabalizing...");
    appendToDisplay("Patient stabalizing...");
}

void AEDController::updateAEDRingState()
{

    // reflect image of next step and then output it, logic will be implemented here as well, to see if they can move to the next step
    currState = static_cast<AEDState>((currState + 1) % 7);
    aedRing->updateImage(currState);

    switch (currState)
    {
    case Default:
        appendToDisplay("The current state of the AED is: Default");

        break;
    case AnalyzingResponsiveness:
        appendToDisplay("The current state of the AED is: Analyzing Responsiveness");
        break;

    case EmergencyServices:
        appendToDisplay("The current state of the AED is: Emergency Services");
        break;

    case Breathing:
        appendToDisplay("The current state of the AED is: Breathing");
        break;

    case ElectrodePlacement:
        enableAEDPlacement();
        appendToDisplay("The current state of the AED is: Electrode Placement");
        break;

    case Shock:
          appendToDisplay("The current state of the AED is: Shock");
        break;

    case PostShockCare:
       appendToDisplay("The current state of the AED is: Post Shock Care");
        break;
    }
}

void AEDController::enableAllComponents()
{
    ui.HeartRateView->setVisible(true);
    ui.BatteryView->setVisible(true);
    ui.centralwidget->setVisible(true);
    ui.patientBodyBox->setVisible(true);
    ui.outputTextGroupBox->setVisible(true);
    aedRing->enable();

}

void AEDController::disableAllComponents()
{
    ui.HeartRateView->setVisible(false);
    ui.BatteryView->setVisible(false);
    ui.patientBodyBox->setVisible(false);
    ui.pushButton1->setVisible(true);
    ui.outputTextGroupBox->setVisible(false);
    aedRing->disable();
}
void AEDController::powerDown()
{
    qDebug() << "Power button pressed!";

    // Toggle the visibility of all components
    if (ui.HeartRateView->isVisible())
    {
        battery->stop();
        // this will make it seem like the battery died. The light will no longer be there, and the aed ring will reset back to default.
        currState = Default;
        aedRing->updateImage(currState);
        disableAllComponents();
    }
    else
    {
        battery->start();
        enableAllComponents();
    }
}
