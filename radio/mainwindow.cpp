#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"
#include "options.h"

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

void MainWindow::onOptionsChanged()
{
    Options* options = Options::getInstance();
    int frequency = options->getFrequency();
    int band = options->getBand();

    ui->qwtPlot->setAxisScale(QwtPlot::Axis::xBottom, frequency - band / 2, frequency + band / 2);
    ui->qwtPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
