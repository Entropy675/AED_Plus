#include "AEDPlacement.h"


AEDPlacement::AEDPlacement(QGroupBox* g)
    : window(g)
{
    if(g == nullptr)
        throw("Exception - AEDPlacement initialized with no target QGroupBox.");

    QPixmap aedPadsImage(":/assets/placement-aed-pads0.png");
    aedImage = new QLabel();
    aedImage->setPixmap(aedPadsImage);
    aedImage->setScaledContents(true);
    patientBodyLayout = new QVBoxLayout(g);
    patientBodyLayout->addWidget(aedImage);
    g->setLayout(patientBodyLayout);

    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &AEDPlacement::loopAnimationTillButtonPress);
    //flashTimer->start(Animation loop rate ms);

}


void AEDPlacement::loopAnimationTillButtonPress()
{
    // flash the current image between currentState and nextState
    // after some callback signal confirms the state change, stop and switch state
}

void AEDPlacement::start()
{
    flashTimer->start(AED_DEMO_LOOP_RATE_MS);
}
