#include "AEDPlacement.h"
#include "defs.h"


AEDPlacement::AEDPlacement(QGroupBox* g)
    : window(g)
{
    if(g == nullptr)
        throw("Exception - AEDPlacement initialized with no target QGroupBox.");


    // Load the images into the array
    for (int i = 0; i < 5; ++i) {
        QString imagePath = QString(":/assets/placement-aed-pads%1.png").arg(i);
        aedPadsImages[i] = QPixmap(imagePath);
    }

    aedImage = new QLabel();
    aedImage->setPixmap(aedPadsImages[NoPatient]);
    aedImage->setScaledContents(true);
    patientBodyLayout = new QVBoxLayout(g);
    patientBodyLayout->addWidget(aedImage);

    buttonLeft = new QPushButton("Place Pad Left");
    buttonRight = new QPushButton("Place Pad Right");

    connect(buttonLeft, &QPushButton::clicked, this, &AEDPlacement::placePadLeft);
    connect(buttonRight, &QPushButton::clicked, this, &AEDPlacement::placePadRight);

    HButtonBox = new QHBoxLayout();
    HButtonBox->addWidget(buttonLeft);
    HButtonBox->addWidget(buttonRight);
    patientBodyLayout->addLayout(HButtonBox);

    g->setLayout(patientBodyLayout);

    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &AEDPlacement::loopAnimationTillButtonPress);
    start();
}


void AEDPlacement::loopAnimationTillButtonPress()
{
    // flash the current image between currentState and nextState
    // after some callback signal confirms the state change, stop and switch state
    if(flip)
    {
        aedImage->setPixmap(aedPadsImages[currentState]);
    }
    else
    {
        aedImage->setPixmap(aedPadsImages[nextState]);
    }
    flip = !flip;
}

void AEDPlacement::placePadLeft()
{
    qDebug() << "Pad left pressed";
    if(currentState != NoPads && nextState != PadLeft)
        return;

    currentState = nextState;
}

void AEDPlacement::placePadRight()
{
    qDebug() << "Pad right pressed";
}

void AEDPlacement::start()
{
    flashTimer->start(AED_DEMO_LOOP_RATE_MS);
}
