#include "AEDPlacement.h"


AEDPlacement::AEDPlacement(QGroupBox* g)
    : window(g)
{
    if(g == nullptr)
        throw("Exception - AEDPlacement initialized with no target QGroupBox.");

    QPixmap aedPadsImage(":/assets/placement-aed-pads0.png");
    aedImage = new QLabel();
    aedImage->setPixmap(aedPadsImage);
    aedImage->setScaledContents(true);
    patientBodyLayout = new QVBoxLayout(g);
    patientBodyLayout->addWidget(aedImage);
    g->setLayout(patientBodyLayout);
}
