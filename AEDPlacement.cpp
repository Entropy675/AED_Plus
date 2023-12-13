#include "AEDPlacement.h"
#include "defs.h"
#include <QDebug>


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

    HButtonBox = new QHBoxLayout();
    patientBodyLayout->addLayout(HButtonBox);

    buttonMid = new QPushButton("Attach AED to Patient");
    connect(buttonMid, &QPushButton::clicked, this, &AEDPlacement::attachedToPatient);
    HButtonBox->addWidget(buttonMid);

    g->setLayout(patientBodyLayout);

    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &AEDPlacement::loopAnimationTillButtonPress);
    flashTimer->start(AED_DEMO_LOOP_RATE_MS);
}


void AEDPlacement::loopAnimationTillButtonPress()
{
    // flash the current image between currentState and nextState
    // after some callback signal confirms the state change, stop and switch state
    if(flip)
        aedImage->setPixmap(aedPadsImages[currentState]);
    else
        aedImage->setPixmap(aedPadsImages[nextState]);
    flip = !flip;
}

void AEDPlacement::placePadLeft()
{
    qDebug() << "Pad left pressed";
    if(currentState != NoPads && nextState != PadLeft)
        return;

    HButtonBox->removeWidget(buttonLeft);
    disconnect(buttonLeft, &QPushButton::clicked, this, &AEDPlacement::placePadLeft);
    delete buttonLeft;


    currentState = nextState;
    nextState = PadBoth;
}

void AEDPlacement::attachedToPatient()
{
    HButtonBox->removeWidget(buttonMid);
    disconnect(buttonMid, &QPushButton::clicked, this, &AEDPlacement::attachedToPatient);
    delete buttonMid;

    buttonLeft = new QPushButton("Place Pad Left");
    buttonRight = new QPushButton("Place Pad Right");
    connect(buttonLeft, &QPushButton::clicked, this, &AEDPlacement::placePadLeft);
    connect(buttonRight, &QPushButton::clicked, this, &AEDPlacement::placePadRight);
    HButtonBox->addWidget(buttonLeft);
    HButtonBox->addWidget(buttonRight);

    currentState = nextState;
    nextState = PadLeft;
    emit AEDAttachedToPatient();
}

void AEDPlacement::placePadRight()
{
    qDebug() << "Pad right pressed";
    if(currentState != PadLeft && nextState != PadBoth)
        return;

    HButtonBox->removeWidget(buttonRight);
    disconnect(buttonRight, &QPushButton::clicked, this, &AEDPlacement::placePadRight);
    delete buttonRight;

    currentState = nextState;
    // now both states should be PadBoth, meaning the patient is padded.
    buttonMid = new QPushButton("Shock Patient!");
    connect(buttonMid, &QPushButton::clicked, this, &AEDPlacement::electrocutePressed);
    HButtonBox->addWidget(buttonMid);
}

void AEDPlacement::electrocutePressed()
{
    emit electrocutePatientPressed();
}

bool AEDPlacement::isDone() // utility function for controller to know when it can administer shock
{
    if(currentState != PadBoth)
        return false;
    return currentState == nextState;
}
