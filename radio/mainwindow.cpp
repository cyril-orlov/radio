#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionsdialog.h"
#include "options.h"
#include <QFileDialog>
#include <QStringListModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qwtPlot = new CustomPlot(this);
    qwtPlot->setContentsMargins(11, 11, 11, 11);
    ui->scrollArea->setWidget(qwtPlot);
}

void MainWindow::optionsClicked_internal()
{
    emit optionsClicked();
}

void MainWindow::stopClicked_internal()
{
    emit stopClicked();
}

void MainWindow::startClicked_internal()
{    
    ListenDialog ld;
    ld.setModal(true);
    if(ld.exec())
        emit startClicked(ld.getTime());
}

void MainWindow::listenChanged(bool value)
{
    ui->actionStart->setEnabled(!value);
    ui->actionStop->setEnabled(value);
    ui->actionChange->setEnabled(!value);
}

void MainWindow::onChartChanged(FilterResult* data)
{
    QwtPlotSpectrogram* curve = dynamic_cast<QwtPlotSpectrogram*>(qwtPlot->itemList()[0]);
    if(curve == nullptr)
        return;

    DataHelper* curveData = static_cast<DataHelper*>(curve->data());
    curveData->setData(data);
    qwtPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
