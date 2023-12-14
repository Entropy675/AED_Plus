#ifndef AEDCOMPRESSIONBAR_H
#define AEDCOMPRESSIONBAR_H

#include <QWidget> // Include any necessary headers
#include <QPushButton>
#include <Qlabel>
#include <QVBoxLayout>
#include <QGroupBox>

class AEDCompressionBar : public QWidget
{
    Q_OBJECT

public:
    AEDCompressionBar(QGroupBox* window = nullptr);
    void updateImage(int);

private:
        QGroupBox* window;
        QLabel* cprImage;
        QPixmap aedCPRImages[9];
        QVBoxLayout *cprLayout;

};

#endif // AEDCOMPRESSIONBAR_H
