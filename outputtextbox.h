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

    void displayText(QString s); // erases
    void addMessage(QString s);  // appends

};

#endif // OUTPUTTEXTBOX_H
