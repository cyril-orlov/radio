#include "mainwindow.h"
#include "timer.h"
#include "options.h"
#include "optionsdialog.h"
#include "receiver.h"
#include "fftransformer.h"
#include "noisegen.hpp"

#include <QApplication>
#include <QTime>
#include <QThread>

void testPlot(MainWindow &w);

int main(int argc, char *argv[])
{
    Options::create();

    QApplication a(argc, argv);
    qRegisterMetaType<QVector<double>>("QVector<double>&");
    qRegisterMetaType<size_t>("size_t");

    MainWindow w;
    OptionsDialog d;
    QObject::connect(&w, &MainWindow::optionsClicked, &d, &OptionsDialog::exec);
    QObject::connect(&d, &OptionsDialog::optionsUpdated, &w, &MainWindow::onOptionsChanged);
    Timer timer(&w);
    timer.work();
    QObject::connect(&timer, &Timer::done, &w, &MainWindow::timerDone);
    QObject::connect(&timer, &Timer::tick, &w, &MainWindow::timerUpdate);
    QObject::connect(&d, &OptionsDialog::accepted, &timer, &Timer::checkOptions);

    FFTransformer fft(Options::getInstance()->getFFTWindow());
    QObject::connect(&fft, &FFTransformer::dataProcessed, &w, &MainWindow::onChartChanged);

    Receiver r;

    NoiseGen ng;
    QThread* ngThread = nullptr;

    if(argc > 1 && QString(argv[1]).compare("-testrandom") == 0)
    {
        ngThread = new QThread();
        ng.moveToThread(ngThread);
        QObject::connect(ngThread, &QThread::started, &ng, &NoiseGen::onStart);
        QObject::connect(&ng, &NoiseGen::received, &fft, &FFTransformer::onDataReceived);
        ngThread->start();
    }
    else
    {
        QObject::connect(&d, &OptionsDialog::optionsUpdated, &r, &Receiver::onOptionsUpdated);
        QObject::connect(&timer, &Timer::done, &r, &Receiver::onStarted);
        QObject::connect(&r, &Receiver::dataReceived, &fft, &FFTransformer::onDataReceived);
    }

    w.show();

    int result = a.exec();

    if(ngThread)
    {
        ng.deactivate();
        ngThread->quit();
        ngThread->wait();
        delete ngThread;
    }

    return result;
}
