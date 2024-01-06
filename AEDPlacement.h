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

    void powerOn();
    void powerOff();
    bool isOn();

    void stopFlashingAnimation();
    void startFlashingAnimation();

    void AEDReadyToBeAttached();
    void ShockButtonReady();
    void ShockButtonDisabled();
    bool AEDIsConnected();

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
    QWidget* leftRightButtonWidget;
    QHBoxLayout * leftRightButtonLayout;
    QWidget* shockContainer;
    QPushButton* buttonLeft;
    QPushButton* buttonMid;
    QPushButton* buttonShock;
    QPushButton* buttonRight;
    bool flip = false;
    bool flashAnimation = true;
    bool power = false;
    bool padsAreAttached = false;

    void startButtonPlacement();
    void leftRightButtonsPlacement();

    int index;
    AEDPlacementState currentState = AEDPlacementState::NoPatient, nextState = AEDPlacementState::NoPads;

    QTimer* flashTimer; // activate to flash till button
};

#endif // AEDPLACEMENT_H
