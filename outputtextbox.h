#ifndef OUTPUTTEXTBOX_H
#define OUTPUTTEXTBOX_H
#include <QObject>
#include <QTextBrowser>

class OutputTextbox : public QTextBrowser
{
    Q_OBJECT

public:
    OutputTextbox(QWidget* parent = nullptr);



};

#endif // OUTPUTTEXTBOX_H
