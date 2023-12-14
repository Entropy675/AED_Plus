#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "AEDController.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void screenResized(int width, int height);

protected:
    void resizeEvent(QResizeEvent* event) override;


private slots:
    void on_powerButton_clicked();

    void on_cprButton_clicked();

private:
    Ui::MainWindow *ui;
    AEDController* aed;

    void setGUILayout();
};
#endif // MAINWINDOW_H
