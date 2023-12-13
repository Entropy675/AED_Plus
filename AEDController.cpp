#include "AEDController.h"
#include <QVBoxLayout>
#include <QPixmap>
#include <QLabel>
AEDController::AEDController(Ui::MainWindow& u)
    : ui(u)
{
    battery = new Battery(u.BatteryView);
    battery->start();
    connect(battery, &Battery::batteryLevelChanged, this, &AEDController::batterydead);

    hMonitor = new HeartRateMonitor(ui.centralwidget, u.HeartRateView->width(), u.HeartRateView->height());
    u.HeartRateView->setScene(hMonitor);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &AEDController::update);
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
       ui.pushButton1->setStyleSheet(
                   "QPushButton {"
                   "   background-color: #4CAF50; /* Green background */"
                   "   border: none; /* Remove border */"
                   "   color: white; /* White text */"
                   "   padding: 10px; /* Add padding */"
                   "   text-align: center; /* Center text */"
                   "   text-decoration: none; /* Remove underline */"
                   "   display: inline-block; /* Display as inline block */"
                   "   font-size: 14px; /* Adjusted font size */"
                   "   margin: 4px 2px; /* Add margin */"
                   "   cursor: pointer; /* Add a cursor on hover */"
                   "   border-radius: 50%; /* Circular shape */"
                   "}"

                   "QPushButton:hover {"
                   "   background-color: #45a049; /* Darker green on hover */"
                   "}"
           );


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
    qDebug("electrocutePressed...");
    if(state != Shock)
        return;

    qDebug("Shock is delivered to the patient!!!!");
}

// occurs each tick...
void AEDController::update()
{
    // qDebug() << " AED CONTROLLER: Update tick... ";

    switch(state)
    {
    case PhysicalError:
        break;
    case StandClear:
        break;
    case PowerOff:
        if (isPowerDown)
                {

                    isPowerDown = false; // Reset the power-down flag
                }
        break;
    case Analyzing:
        break;
    case ShockAdvised:
        break;
    case Shock:
        break;
    case PostShockCare:
        break;
    case ContinuedEvaluation:
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
