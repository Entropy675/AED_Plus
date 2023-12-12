#include "AEDRing.h"

AEDRing::AEDRing(QGroupBox *view) : window(view), currState(AEDState::ContinuedEvaluation)
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
    originalState->addWidget(updateButton);
    connect(updateButton, &QPushButton::clicked, this, &AEDRing::updateButtonClicked);


    window->setLayout(originalState);

}

void AEDRing::updateButtonClicked() {

    currState = static_cast<AEDState>((currState + 1) % 7);

    // Update the displayed image
    aedImage->setPixmap(aedStateImages[currState]);

    // Emit the signal to notify about the state change
    emit updateAEDState(currState);
}

