#include "AEDController.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
AEDController::AEDController(Ui::MainWindow& u)
    : ui(u), currState(Default)
{
    battery = new Battery(u.BatteryView);
    battery->start();
    connect(battery, &Battery::batteryLevelChanged, this, &AEDController::batterydead);
    hMonitor = new HeartRateMonitor(nullptr, u.bpmNumber, u.HeartRateView->width(), u.HeartRateView->height());
    u.HeartRateView->setScene(hMonitor);

    updateTimer = new QTimer(this);
    //connect(updateTimer, &QTimer::timeout, this, &AEDController::update); ????? why use update
    updateTimer->start(PING_RATE_MS);

    connect(ui.pushButton1, &QPushButton::clicked, this, &AEDController::powerDown);
    // connect signal from HeartRateMonitor to this classes slot
    connect(hMonitor, &HeartRateMonitor::pushTextToDisplay, this, &AEDController::appendToDisplay);

    outputText = new OutputTextbox(ui.outputTextGroupBox);

    QVBoxLayout* outputBoxLayout = new QVBoxLayout();
    outputBoxLayout->addWidget(outputText);
    ui.outputTextGroupBox->setLayout(outputBoxLayout);

    aedPlacementDemo = new AEDPlacement(ui.patientBodyBox);
    connect(aedPlacementDemo, &AEDPlacement::AEDAttachedToPatient, this, &AEDController::AEDAttachedStartAnalyzing);
    connect(aedPlacementDemo, &AEDPlacement::electrocutePatientPressed, this, &AEDController::electrocutePressed);

    QVBoxLayout* leftSideLayout = new QVBoxLayout();

     // Add your new widgets or components to the left side layout
     // For example, add a QLabel
     QLabel* leftSideLabel = new QLabel("Left Side Content");
     leftSideLayout->addWidget(leftSideLabel);

//       ui.centralWidget->layout()->addLayout(leftSideLayout);

     QPixmap powerButtonImage(":/assets/powerButton.jpg");
     ui.pushButton1->setIcon(powerButtonImage);
     ui.pushButton1->setIconSize(QSize(30, 30));
  
    aedRing = new AEDRing(ui.AEDRingView);
    connect(aedRing, &AEDRing::updateAEDState, this, &AEDController::updateAEDRingState);

    battery = new Battery(u.BatteryView);

    connect(ui.powerButton, &QPushButton::clicked, this, &AEDController::powerDown);

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

    qDebug("Shock is delivered to the patient!!!!");
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
    ui.powerButton->setVisible(true);
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
void AEDController::batterydead()
{
    if (battery->getBatteryLevel() == 0) {
        ui.HeartRateView->setVisible(false);
        ui.pushButton1->setVisible(false);
        ui.outputTextGroupBox->setVisible(false);
        ui.BatteryView->setVisible(true);
        ui.patientBodyBox->setVisible(false);
    }else {
        ui.pushButton1->setVisible(true);
    }
}
