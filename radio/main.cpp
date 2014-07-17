#include "mainwindow.h"
#include "timer.h"
#include "options.h"
#include "optionsdialog.h"
#include "receiver.h"
#include "fftransformer.h"

#include <QApplication>

void testPlot(MainWindow &w);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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

    Receiver r;
    QObject::connect(&d, &OptionsDialog::optionsUpdated, &r, &Receiver::onOptionsUpdated);
    QObject::connect(&timer, &Timer::done, &r, &Receiver::onStarted);

    FFTransformer fft((size_t)Options::getInstance()->getBand(), &w);
    QObject::connect(&r, &Receiver::dataReceived, &fft, &FFTransformer::onDataReceived);
    QObject::connect(&d, &OptionsDialog::optionsUpdated, &fft, &FFTransformer::init);

    w.show();

    testPlot(w);

    return a.exec();
}

void testPlot(MainWindow& w)
{
    FFTransformer fft(8, &w);
    Complex* testData = new Complex[8];
    testData[0] = Complex(0, 1);
    testData[1] = Complex(1, 3);
    testData[2] = Complex(2, 1);
    testData[3] = Complex(3, 3);
    testData[4] = Complex(4, 1);
    testData[5] = Complex(5, 3);
    testData[6] = Complex(6, 1);
    testData[7] = Complex(7, 3);
    fft.onDataReceived(testData, 8);
}
