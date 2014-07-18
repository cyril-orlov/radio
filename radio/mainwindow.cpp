#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"
#include "options.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qwtPlot = new CustomPlot(this);
    ui->horizontalLayout_2->addWidget(qwtPlot);
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

    qwtPlot->setAxisScale(QwtPlot::Axis::xBottom, frequency - band / 2, frequency + band / 2);
}

void MainWindow::onChartChanged(QVector<double> data)
{
    QwtPlotCurve* curve = dynamic_cast<QwtPlotCurve*>(qwtPlot->itemList()[0]);
    if(curve == nullptr)
        return;

    DataHelper* curveData = static_cast<DataHelper*>(curve->data());
    curveData->setData(data);
}

MainWindow::~MainWindow()
{
    delete ui;
}
