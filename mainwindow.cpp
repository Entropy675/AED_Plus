#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("AED Plus");
    aed = new AEDController(*ui);

    connect(this, &MainWindow::screenResized, aed, &AEDController::handleScreenResized);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete aed;
}


void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); // base implementation
    emit screenResized(event->size().width(), event->size().height()); // Emit to ec with new screen dimensions
}
