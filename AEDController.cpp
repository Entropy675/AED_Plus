#include "AEDController.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
AEDController::AEDController(Ui::MainWindow& u)
    : ui(u), isPowerDown(true)
{
    battery = new Battery(u.BatteryView);
    connect(battery, &Battery::batteryLevelChanged, this, &AEDController::batterydead);
    battery->start();

    hMonitor = new HeartRateMonitor(nullptr, u.bpmNumber, u.HeartRateView->width(), u.HeartRateView->height());
    // connect signal from HeartRateMonitor to this classes slot
    connect(hMonitor, &HeartRateMonitor::pushTextToDisplay, this, &AEDController::appendToDisplay);
    connect(u.scenarioSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AEDController::heartRhythmChanged);
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

    QPixmap powerButtonImage(":/assets/powerButton.jpg");
    ui.powerButton->setIcon(powerButtonImage);
    ui.powerButton->setIconSize(QSize(30, 30));
    //=======
    //powerButtonImageOn.load(":/assets/powerButtonOn.png");
    //powerButtonImageOff.load(":/assets/powerButtonOff.png");
    //ui.powerButton->setIcon(powerButtonImageOff);
    //>>>>>>> dev

    aedRing = new AEDRing(ui.AEDRingView);
    connect(aedRing, &AEDRing::updateAEDState, this, &AEDController::updateAEDRingState);

    battery = new Battery(u.BatteryView);

    connect(ui.powerButton, &QPushButton::clicked, this, &AEDController::power);

    battery->start();
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
    outputText->append(s);
}

void AEDController::AEDAttachedStartAnalyzing()
{
    if(!hMonitor->isOn())
        hMonitor->powerOn();
    hMonitor->startAnalyzing();
}

void AEDController::electrocutePressed()
{
    if(aedRing->getState() != AEDRing::Shock) {
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


void AEDController::resetHeartbeat()
{

    qDebug("Shock is delivered to the patient!!!!");
}

void AEDController::updateAEDRingState()
{

    // reflect image of next step and then output it, logic will be implemented here as well, to see if they can move to the next step
    aedRing->setState(static_cast<AEDRing::AEDState>((aedRing->getState() + 1) % 7));

    aedRing->updateImage(aedRing->getState());

    switch (aedRing->getState())
    {
    case AEDRing::Default:
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
        aedPlacementDemo->powerOn();
        appendToDisplay("The current state of the AED is: Electrode Placement");
        break;

    case AEDRing::Shock:
          appendToDisplay("The current state of the AED is: Shock");
        break;

    case AEDRing::PostShockCare:
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
void AEDController::power()
{
    if (isPowerDown)
        isPowerDown = false;
    else {
         isPowerDown = true;
    }

    // Toggle the visibility of all components
    if (isPowerDown)
    {
        appendToDisplay("Power off");
        ui.powerButton->setIcon(powerButtonImageOff);
        battery->stop();
        // this will make it seem like the battery died. The light will no longer be there, and the aed ring will reset back to default.
        aedRing->setState(AEDRing::Default);
        aedRing->updateImage(aedRing->getState());
        disableAllComponents();
    }
    else if (!isPowerDown)
    {
        appendToDisplay("Power on");
        ui.powerButton->setIcon(powerButtonImageOn);
        battery->start();
        enableAllComponents();
    }
}
void AEDController::batterydead()
{
    if (battery->getBatteryLevel() == 0) {
        ui.HeartRateView->setVisible(false);
        ui.powerButton->setVisible(false);
        ui.outputTextGroupBox->setVisible(false);
        ui.BatteryView->setVisible(true);
        ui.patientBodyBox->setVisible(false);
    }else {
        ui.powerButton->setVisible(true);
    }
}


