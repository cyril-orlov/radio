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

    FFTransformer fft(&w);
    QObject::connect(&r, &Receiver::dataReceived, &fft, &FFTransformer::onDataReceived);

    w.show();

    testPlot(w);

    return a.exec();
}

void testPlot(MainWindow& w)
{
    FFTransformer fft(&w);
    QVector<Complex>* testData = new QVector<Complex>;
    testData->append(Complex(0, 1));
    testData->append(Complex(1, 3));
    testData->append(Complex(2, 1));
    testData->append(Complex(3, 3));
    testData->append(Complex(4, 1));
    testData->append(Complex(5, 3));
    testData->append(Complex(6, 1));
    testData->append(Complex(7, 3));
    fft.onDataReceived(testData);
}
