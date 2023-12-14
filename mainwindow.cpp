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

    setGUILayout();
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
void MainWindow::setGUILayout() {

    QVBoxLayout* verticalLayout = new QVBoxLayout(ui->centralwidget);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();

    verticalLayout->addStretch();

    verticalLayout->addLayout(horizontalLayout);


    ui->powerButton->setIconSize(ui->powerButton->icon().actualSize(QSize(140, 140)));


    ui->powerButton->setFixedSize(ui->powerButton->iconSize());


    horizontalLayout->addWidget(ui->powerButton);


    verticalLayout->addStretch();

    horizontalLayout->addSpacerItem(new QSpacerItem(320, 0, QSizePolicy::Fixed, QSizePolicy::Minimum));
    verticalLayout->addSpacerItem(new QSpacerItem(0, 210, QSizePolicy::Fixed, QSizePolicy::Minimum));

    ui->centralwidget->setLayout(verticalLayout);


    this->setCentralWidget(ui->centralwidget);


    this->show();
}


void MainWindow::on_powerButton_clicked()
{

}


void MainWindow::on_cprButton_clicked()
{

}

