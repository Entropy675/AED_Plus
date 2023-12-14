#include "AEDRing.h"

AEDRing::AEDRing(QGroupBox *view) : window(view), currState(AnalyzingResponsiveness)
{
    for (int i = 0; i < 7; ++i) {
        QString imagePath = QString(":/assets/AED_RING_STATE%1.png").arg(i);
        aedStateImages[i] = QPixmap(imagePath);
    }

    aedImage = new QLabel();
    aedImage->setPixmap(aedStateImages[0]);
    aedImage->setScaledContents(true);
    originalState = new QVBoxLayout(window);
    originalState->addWidget(aedImage);
    updateButton = new QPushButton("Go To Next State", this);
    connect(updateButton, &QPushButton::clicked, this, &AEDRing::updateButtonClicked);
    originalState->addWidget(updateButton);

    window->setLayout(originalState);
}

AEDRing::~AEDRing()
{
    delete originalState;
}


AEDRing::AEDState AEDRing::getState(){
    return currState;
}

void AEDRing::setState(AEDRing::AEDState stateToSetTo) {
    currState = stateToSetTo;
}

void AEDRing::updateButtonClicked() {
    emit updateAEDState();
}


void AEDRing::updateImage(AEDState state) {
    aedImage->setPixmap(aedStateImages[state]);
}

void AEDRing::disable() {
    updateButton->setDisabled(true);
}

void AEDRing::enable() {
    updateButton->setDisabled(false);
}

