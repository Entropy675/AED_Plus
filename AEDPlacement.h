#ifndef AEDPLACEMENT_H
#define AEDPLACEMENT_H

#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QPushButton>

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
    bool isDone();

    void stopFlashingAnimation();
    void startFlashingAnimation();

signals:
    void pushTextToDisplay(QString s);
    void AEDAttachedToPatient();
    void electrocutePatientPressed();

private slots:
    void loopAnimationTillButtonPress();
    void placePadLeft();
    void attachedToPatient();
    void placePadRight();
    void electrocutePressed();

private:
    const QGroupBox* window;
    QLabel* aedImage;
    QPixmap aedPadsImages[5];
    QVBoxLayout* patientBodyLayout;
    QHBoxLayout* HButtonBox;
    QPushButton* buttonLeft;
    QPushButton* buttonMid;
    QPushButton* buttonRight;
    bool flip = false;
    bool flashAnimation = true;

    AEDPlacementState currentState = AEDPlacementState::NoPatient, nextState = AEDPlacementState::NoPads;

    QTimer* flashTimer; // activate to flash till button
};

#endif // AEDPLACEMENT_H
