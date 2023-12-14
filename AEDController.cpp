#include "AEDController.h"

#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
#include <QDateTime>
AEDController::AEDController(Ui::MainWindow& u)
    : ui(u), isPowerOn(false)
{
    hMonitor = new HeartRateMonitor(nullptr, u.bpmNumber, u.HeartRateView->width(), u.HeartRateView->height());
    // connect signal from HeartRateMonitor to this classes slot
    connect(hMonitor, &HeartRateMonitor::pushTextToDisplay, this, &AEDController::appendToDisplay);
    connect(u.heartRhythmSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AEDController::heartRhythmChanged);
    u.HeartRateView->setScene(hMonitor);


    //updateTimer = new QTimer(this);
    //connect(updateTimer, &QTimer::timeout, this, &AEDController::update); ????? why use update
    //updateTimer->start(PING_RATE_MS);

    outputText = new OutputTextbox(ui.outputTextGroupBox);

    QVBoxLayout* outputBoxLayout = new QVBoxLayout();
    outputBoxLayout->addWidget(outputText);
    ui.outputTextGroupBox->setLayout(outputBoxLayout);

    aedPlacementDemo = new AEDPlacement(ui.patientBodyBox);
    connect(aedPlacementDemo, &AEDPlacement::pushTextToDisplay, this, &AEDController::appendToDisplay);
    connect(aedPlacementDemo, &AEDPlacement::AEDAttachedToPatient, this, &AEDController::AEDAttachedStartAnalyzing);
    connect(aedPlacementDemo, &AEDPlacement::electrocutePatientPressed, this, &AEDController::electrocutePressed);

    QVBoxLayout* leftSideLayout = new QVBoxLayout();

    // Add your new widgets or components to the left side layout
    // For example, add a QLabel
    QLabel* leftSideLabel = new QLabel("Left Side Content");
    leftSideLayout->addWidget(leftSideLabel);

    //ui.centralWidget->layout()->addLayout(leftSideLayout);

    ui.powerButton->setIconSize(QSize(30, 30));

    powerButtonImageOn.load(":/assets/powerButtonOn.png");
    powerButtonImageOff.load(":/assets/powerButtonOff.png");

    ui.powerButton->setIcon(powerButtonImageOff);
    ui.heartRhythmSelector->setEnabled(false);

    aedRing = new AEDRing(ui.AEDRingView);
    connect(aedRing, &AEDRing::updateAEDState, this, &AEDController::updateAEDRingState);

    battery = new Battery(u.BatteryView);
    connect(battery, &Battery::batteryLevelChanged, this, &AEDController::batterydead);
    connect(ui.powerButton, &QPushButton::clicked, this, &AEDController::power);

}

AEDController::~AEDController()
{
    delete hMonitor;
    delete outputText;
    delete aedPlacementDemo;
}

void AEDController::heartRhythmChanged(int index)
{
    qDebug() << "Heart rhythm changed";
    switch(index)
    {
    case 0:
        hMonitor->changeRhythm(HeartRateMonitor::PEA);
        break;

    case 1:
        hMonitor->changeRhythm(HeartRateMonitor::ASYSTOLE);
        break;

    case 2:
        hMonitor->changeRhythm(HeartRateMonitor::VF);
        break;

    case 3:
        hMonitor->changeRhythm(HeartRateMonitor::VT);
        break;
    default:
        return;
    }
}

void AEDController::handleScreenResized(int w, int h)
{
    // we could rescale the UI element sizes here...
    // new window widths are w and h
}

void AEDController::appendToDisplay(QString s)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss AP");
    QString messageWithTimestamp = timestamp + " - " + s;
    outputText->append(messageWithTimestamp);
    // outputText->append(s);
}

void AEDController::AEDAttachedStartAnalyzing()
{
    if(!hMonitor->isOn())
        hMonitor->powerOn();
    hMonitor->startAnalyzing();
}

void AEDController::electrocutePressed()
{
    // heart rhythms that allow for a shock
    if (ui.heartRhythmSelector->currentText() == "Ventricular Tachycardia" || ui.heartRhythmSelector->currentText() == "Ventricular Fibrillation") {
        appendToDisplay("Electricution delivered!");
        hMonitor->updateHeartRate(300);
    }

}


void AEDController::updateAEDRingState()
{

    // reflect image of next step and then output it, logic will be implemented here as well, to see if they can move to the next step
    if (aedRing->getState()== AEDRing::ElectrodePlacement) {
        if (!aedPlacementDemo->AEDIsConnected()){
            return;
        }
    }
    aedRing->setState(static_cast<AEDRing::AEDState>((aedRing->getState() + 1) % 7));

    aedRing->updateImage(aedRing->getState());

    switch (aedRing->getState())
    {
    case AEDRing::Default:
        power();
        appendToDisplay("The current state of the AED is: Default");
        break;
        
    case AEDRing::AnalyzingResponsiveness:
        appendToDisplay("The current state of the AED is: Analyzing Responsiveness");
        break;
        
    case AEDRing::EmergencyServices:
        appendToDisplay("The current state of the AED is: Emergency Services");
        break;

    case AEDRing::Breathing:
        appendToDisplay("The current state of the AED is: Breathing");
        break;

    case AEDRing::ElectrodePlacement:
        aedPlacementDemo->AEDReadyToBeAttached();
        appendToDisplay("The current state of the AED is: Electrode Placement");
        break;

    case AEDRing::Shock:
        aedPlacementDemo->AEDReadyToBeAttached();
          appendToDisplay("The current state of the AED is: Shock");
        break;

    case AEDRing::PostShockCare:
       appendToDisplay("The current state of the AED is: Post Shock Care");
        break;
    }
}

void AEDController::enableAllComponents()
{
    appendToDisplay("Power On");
    turnPowerButtonOn();
    aedPlacementDemo->powerOn();
    hMonitor->powerOn();
    ui.BatteryView->setVisible(true);
    ui.heartRhythmSelector->setEnabled(true);
    aedRing->enable();
    battery->start();
}



void AEDController::disableAllComponents()

{   appendToDisplay("Power Off");
    turnPowerButtonOff();
    aedPlacementDemo->powerOff();
    hMonitor->powerOff();
    ui.heartRhythmSelector->setEnabled(false);
    outputText->setText("");
    battery->stop();
    aedRing->disable();
}
void AEDController::power()
{
    if (isPowerOn) {
        isPowerOn = false;
    }
    else if (!isPowerOn){
         isPowerOn = true;
    }

    // Toggle the visibility of all components
    if (!isPowerOn)
    {
        // this will make it seem like the battery died. The light will no longer be there, and the aed ring will reset back to default
        disableAllComponents();
    }
    else if (isPowerOn)
    {
        enableAllComponents();
    }
}

void AEDController::batterydead()
{
    if (battery->getBatteryLevel() == 0)
        disableAllComponents();
}


void AEDController::turnPowerButtonOn() {
    ui.powerButton->setIcon(powerButtonImageOn);
}


void AEDController::turnPowerButtonOff() {
    ui.powerButton->setIcon(powerButtonImageOff);
}

