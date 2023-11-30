#ifndef OUTPUTTEXTBOX_H
#define OUTPUTTEXTBOX_H
#include <QObject>
#include <QTextBrowser>

class OutputTextbox : public QTextBrowser
{
    Q_OBJECT

public:
    OutputTextbox(QWidget* parent = nullptr);
    ~OutputTextbox();
    // use QString, compatibility with Qt
    // this->setText("New Text");
    // this->append("Additional Text");
};

#endif // OUTPUTTEXTBOX_H
