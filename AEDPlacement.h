#ifndef AEDPLACEMENT_H
#define AEDPLACEMENT_H

#include "defs.h"

#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>

class AEDPlacement : public QObject
{
    Q_OBJECT

public:
    enum AEDPlacementState
    {
        NoPatient,
        NoPads,
        PadLeft,
        PadRight,
        PadBoth
    };
    // before switching state, flash the state,
    // wait for button press, then switch

    AEDPlacement(QGroupBox* g = nullptr);
    void start();

private slots:
    void loopAnimationTillButtonPress();

private:
    const QGroupBox* window;
    QLabel* aedImage;
    QVBoxLayout* patientBodyLayout;

    AEDPlacementState currentState = AEDPlacementState::NoPatient, nextState = AEDPlacementState::NoPads;

    QTimer* flashTimer; // activate to flash till button
};

#endif // AEDPLACEMENT_H
