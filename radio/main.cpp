#include "mainwindow.h"
#include "timer.h"
#include "options.h"
#include "optionsdialog.h"
#include "receiver.h"

#include <QApplication>

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

    w.show();

    return a.exec();
}
