#include "AEDController.h"
#include <QVBoxLayout>

AEDController::AEDController(Ui::MainWindow& u)
    : ui(u)
{
    hMonitor = new HeartRateMonitor(nullptr, START_HEART_RATE, u.HeartRateView->width(), u.HeartRateView->height());
    u.HeartRateView->setScene(hMonitor);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &AEDController::update);
    updateTimer->start(PING_RATE_MS);

    // connect signal from HeartRateMonitor to this classes slot
    connect(hMonitor, &HeartRateMonitor::pushTextToDisplay, this, &AEDController::appendToDisplay);

    outputText = new OutputTextbox(ui.outputTextGroupBox);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(outputText);
    ui.outputTextGroupBox->setLayout(layout);
}

AEDController::~AEDController()
{
    updateTimer->stop();
    delete updateTimer;

    delete hMonitor;
    delete outputText;
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
