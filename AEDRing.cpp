#include "AEDRing.h"

AEDRing::AEDRing(QGroupBox *view) : window(view)
{
    for (int i = 0; i < 7; ++i) {
        QString imagePath = QString(":/assets/AED_RING_STATE%1.png").arg(i);
        aedStateImages[i] = QPixmap(imagePath);
    }

    aedImage = new QLabel();
    aedImage->setScaledContents(true);
    originalState = new QVBoxLayout(window);
    originalState->addWidget(aedImage);
    updateButton = new QPushButton("Go To Next State", this);
    connect(updateButton, &QPushButton::clicked, this, &AEDRing::updateButtonClicked);
    originalState->addWidget(updateButton);

    window->setLayout(originalState);

    disable();
}

AEDRing::~AEDRing()
{
    delete originalState;
}


void AEDRing::updateButtonClicked() {
    emit updateAEDState();
}


void AEDRing::updateImage(AEDState state) {
    aedImage->setPixmap(aedStateImages[state]);
}

void AEDRing::disable() {
    updateButton->setDisabled(true);
    currState = Default;
    updateImage(currState);
}

void AEDRing::enable() {
    updateButton->setDisabled(false);
    currState = AnalyzingResponsiveness;
    updateImage(currState);
}

AEDRing::AEDState AEDRing::getState() {
    return currState;
}

void AEDRing::setState(AEDState stateToChangeTo) {
    currState = stateToChangeTo;
}
