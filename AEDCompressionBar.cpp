#include "AEDCompressionBar.h"

AEDCompressionBar::AEDCompressionBar(QGroupBox *view) : window(view)
{
    for (int i = 0; i < 9; ++i) {
        QString imagePath = QString(":/assets/AEDCPR%1.png").arg(i);
        aedCPRImages[i] = QPixmap(imagePath);
    }
    cprImage = new QLabel();
    cprImage->setScaledContents(true);
    cprImage->setPixmap(aedCPRImages[0]);
    cprLayout = new QVBoxLayout(window);
    cprLayout->addWidget(cprImage);

    window->setLayout(cprLayout);


}

void AEDCompressionBar::updateImage(int i) {
    cprImage->setPixmap(aedCPRImages[i]);
}



