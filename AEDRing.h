#ifndef AEDRING_H
#define AEDRING_H

#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include "defs.h"

class AEDRing : public QWidget
{
    Q_OBJECT

public:
    AEDRing(QGroupBox* window = nullptr);
    void disable();
    void enable();
    void updateImage(AEDState);

private slots:
void updateButtonClicked();


signals:
void updateAEDState();

private:
    QGroupBox* window;
    QVBoxLayout *originalState;
    QLabel* aedImage;
    QPixmap aedStateImages[7];
    QPushButton *updateButton;

};

#endif // AEDRING_H
