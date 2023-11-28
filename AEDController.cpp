#include "AEDController.h"

AEDController::AEDController(Ui::MainWindow& u)
    : ui(u)
{
    hMonitor = new HeartRateMonitor();
    u.HeartRateView->setScene(hMonitor);
}


void AEDController::handleScreenResized(int w, int h)
{
    // we could change the UI element sizes here...
}
