#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::optionsClicked_internal()
{
    emit optionsClicked();
}

void MainWindow::timerDone()
{
    ui->statusBar->showMessage(QString("Готово"));
}

void MainWindow::timerUpdate(int count)
{
    ui->statusBar->showMessage(QString::number(count));
}

MainWindow::~MainWindow()
{
    delete ui;
}
