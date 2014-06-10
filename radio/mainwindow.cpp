#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"
#include "options.h"
#include <qwt_plot_curve.h>
#include "datahelper.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QwtPlotCurve* curve = new QwtPlotCurve();
    curve->setPen(Qt::blue);
    curve->attach(ui->qwtPlot);
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

void MainWindow::onChartChanged(QVector<Complex> *data)
{
    QwtPlotCurve* curve = dynamic_cast<QwtPlotCurve*>(ui->qwtPlot->itemList()[0]);
    if(curve == nullptr)
        return;

    DataHelper* convertedData = new DataHelper(data);

    curve->setData(convertedData);
}

MainWindow::~MainWindow()
{
    delete ui;
}
