#include "AEDController.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QThread>

AEDController::AEDController(Ui::MainWindow& u)
    : ui(u)
{
    hMonitor = new HeartRateMonitor(nullptr, u.graphicsView->width(), u.graphicsView->height());
    u.graphicsView->setScene(hMonitor);

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

    aedPlacementDemo = new AEDPlacement(ui.patientBodyBox);
    connect(aedPlacementDemo, &AEDPlacement::pushTextToDisplay, this, &AEDController::appendToDisplay);
    connect(aedPlacementDemo, &AEDPlacement::AEDAttachedToPatient, this, &AEDController::AEDAttachedStartAnalyzing);
    connect(aedPlacementDemo, &AEDPlacement::electrocutePatientPressed, this, &AEDController::electrocutePressed);

    aedRingDemo = new AEDRing(ui.AEDRingView);


    connect(aedRingDemo, &AEDRing::updateAEDState, this, &AEDController::updateAEDRingState);

    connect(ui.pushButton1, &QPushButton::clicked, this, &AEDController::powerDown);
    battery = new Battery(u.BatteryView);
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
    //if(state != Shock) // add this back later when the states work
        //return;
    appendToDisplay("Electricution delivered!");
    qDebug("Shock is delivered to the patient!!!!");
    hMonitor->updateHeartRate(300);
    restartHeartbeat->start(1200);
}

void AEDController::resetHeartbeat()
{

    hMonitor->updateHeartRate(START_HEART_RATE);
    qDebug("Patient stabalizing...");
    appendToDisplay("Patient stabalizing...");
}

void AEDController::updateAEDRingState(AEDState stateToUpdateTo)
{
    state = stateToUpdateTo;
    switch (stateToUpdateTo)
    {
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
        appendToDisplay("The current state of the AED is: Electrode Placement");
        break;

    case HeartRythmAnalysis:
        appendToDisplay("The current state of the AED is: Heart Rythym Analsysis");
        break;

    case Shock:
          appendToDisplay("The current state of the AED is: Shock");
        break;

    case PostShockCare:
       appendToDisplay("The current state of the AED is: Post Shock Care");
        break;

    case ContinuedEvaluation:

         appendToDisplay("The current state of the AED is: Continued Evaluation");
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

}

void AEDController::disableAllComponents()
{
    ui.HeartRateView->setVisible(false);
    ui.BatteryView->setVisible(false);
    ui.patientBodyBox->setVisible(false);
    ui.pushButton1->setVisible(true);
    ui.outputTextGroupBox->setVisible(false);
}
void AEDController::powerDown()
{
    qDebug() << "Power button pressed!";

    // Toggle the visibility of all components
    if (ui.HeartRateView->isVisible())
    {
        battery->stop();
        disableAllComponents();
    }
    else
    {
        battery->start();
        enableAllComponents();
    }
}
