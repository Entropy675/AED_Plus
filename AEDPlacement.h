#ifndef AEDPLACEMENT_H
#define AEDPLACEMENT_H

#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

class AEDPlacement : public QObject
{
    Q_OBJECT

public:
    AEDPlacement(QGroupBox* g = nullptr);

private:
    const QGroupBox* window;
    QLabel* aedImage;
    QVBoxLayout* patientBodyLayout;
};

#endif // AEDPLACEMENT_H
