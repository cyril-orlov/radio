#include "mainwindow.h"
#include "timer.h"
#include "optionsdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    OptionsDialog d;
    QObject::connect(&w, &MainWindow::optionsClicked, &d, &OptionsDialog::exec);
    Timer timer(&w);
    timer.work();
    QObject::connect(&timer, &Timer::done, &w, &MainWindow::timerDone);
    QObject::connect(&timer, &Timer::tick, &w, &MainWindow::timerUpdate );
    QObject::connect(&d, &OptionsDialog::accepted, &timer, &Timer::checkOptions);

    w.show();

    return a.exec();
}
