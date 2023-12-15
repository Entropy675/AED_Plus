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

    g->setLayout(patientBodyLayout);

    flashTimer = new QTimer(this);
    connect(flashTimer, &QTimer::timeout, this, &AEDPlacement::loopAnimationTillButtonPress);
    flashTimer->start(AED_DEMO_LOOP_RATE_MS);
    power = false;
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


    leftRightButtonLayout->removeWidget(buttonLeft);
    delete buttonLeft;


    currentState = nextState;
    nextState = PadBoth;
}

void AEDPlacement::AEDReadyToBeAttached() {
    buttonMid->setEnabled(true);
}

void AEDPlacement::ShockButtonReady() {
    buttonShock->setEnabled(true);
}
void AEDPlacement::ShockButtonDisabled() {
    buttonShock->setEnabled(false);
}


bool AEDPlacement::AEDIsConnected(){
    return padsAreAttached;
}

void AEDPlacement::startButtonPlacement()
{
    buttonMid = new QPushButton("Attach AED to Patient");
    buttonMid->setEnabled(false);
    HButtonBox->addWidget(buttonMid);
    connect(buttonMid, &QPushButton::clicked, this, &AEDPlacement::attachedToPatient);
}

void AEDPlacement::powerOn()
{
    power = true;
    HButtonBox = new QHBoxLayout();
    patientBodyLayout->addLayout(HButtonBox);
    startButtonPlacement();;
    // attach button does not turn on right away till it is ready to be attached
   // if(buttonMid)
       // buttonMid->setDisabled(false);
}

void AEDPlacement::powerOff()
{
    power = false;
    // remove all widgets so that we can reset the process when it turns on
    QLayoutItem *widgetToRemoveFromAEDPlacementBox;
    while ((widgetToRemoveFromAEDPlacementBox = HButtonBox->takeAt(0)) != nullptr) {
        QWidget *widget = widgetToRemoveFromAEDPlacementBox->widget();
        if (widget) {
            widget->setParent(nullptr);
            delete widget;
        }
        delete widgetToRemoveFromAEDPlacementBox;
    }
    loopAnimationTillButtonPress();
    currentState = AEDPlacementState::NoPatient;
    nextState = AEDPlacementState::NoPads;
}


bool AEDPlacement::isOn()
{
    return power;
}

void AEDPlacement::attachedToPatient()
{
    index = HButtonBox->indexOf(buttonMid);

    HButtonBox->removeWidget(buttonMid);
    delete buttonMid;
    leftRightButtonsPlacement();


    currentState = nextState;
    nextState = PadLeft;


    emit AEDAttachedToPatient();
}

void AEDPlacement::leftRightButtonsPlacement()
{
    leftRightButtonWidget = new QWidget;

    leftRightButtonLayout = new QHBoxLayout(leftRightButtonWidget);


    buttonLeft = new QPushButton("Place Pad Left");
    buttonRight = new QPushButton("Place Pad Right");


    connect(buttonLeft, &QPushButton::clicked, this, &AEDPlacement::placePadLeft);
    connect(buttonRight, &QPushButton::clicked, this, &AEDPlacement::placePadRight);


    leftRightButtonLayout->addWidget(buttonLeft);
    leftRightButtonLayout->addWidget(buttonRight);

    HButtonBox->insertWidget(index, leftRightButtonWidget);
}

void AEDPlacement::placePadRight()
{
    qDebug() << "Pad right pressed";
    if(currentState != PadLeft && nextState != PadBoth)
        return;

    leftRightButtonLayout->removeWidget(buttonRight);
    delete buttonRight;
    currentState = nextState;
    // now both states should be PadBoth, meaning the patient is padded.
    buttonShock = new QPushButton("Shock Patient!");

    padsAreAttached = true;
    connect(buttonShock, &QPushButton::clicked, this, &AEDPlacement::electrocutePressed);
  //shock should not be administered till in the right AEDRing state
    buttonShock->setEnabled(false);

    HButtonBox->insertWidget(index,buttonShock);


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
