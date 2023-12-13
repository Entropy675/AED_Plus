#ifndef AEDRING_H
#define AEDRING_H

#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <qpushbutton.h>
#include <qboxlayout.h>

class AEDRing : public QWidget
{
    Q_OBJECT

public:

    enum AEDState
    {
        Default,
        AnalyzingResponsiveness,
        EmergencyServices,
        Breathing, // use lid to put behind victims shoulders to maintain an effective airway (dont use support if spinal injury)
        ElectrodePlacement,
        Shock, // shock is also HeartRythmAnalysis,this is where it will be judged
        PostShockCare, // bunched with continued evaluation
    }; // we can add more/remove some as we need


    AEDRing(QGroupBox* window = nullptr);
    void disable();
    void enable();
    void updateImage(AEDState);
    AEDState getState();
    void setState(AEDState);



private slots:
void updateButtonClicked();


signals:
void updateAEDState();

private:
    AEDState currState;
    QGroupBox* window;
    QVBoxLayout *originalState;
    QLabel* aedImage;
    QPixmap aedStateImages[7];
    QPushButton *updateButton;

};

#endif // AEDRING_H
