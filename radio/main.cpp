
#include "mainwindow.h"
#include "options.h"
#include "optionsdialog.h"
#include "receiver.h"
#include "fftransformer.h"
#include "controller.hpp"
#include "testcontroller.hpp"

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

    bool random = 1;//argc == 2 && QString(argv[1]).compare("-random") == 0;
    AbstractController* controller;

    if(random)
        controller = new TestController();
    else
        controller = new Controller();

    QObject::connect(controller,
                     &AbstractController::dataProcessed,
                     &w,
                     &MainWindow::onChartChanged
                     );

    QObject::connect(controller,
                     &AbstractController::listenChanged,
                     &w,
                     &MainWindow::listenChanged
                     );

    QObject::connect(&w,
                     &MainWindow::startClicked,
                     controller,
                     &AbstractController::listen
                     );

    QObject::connect(&w,
                     &MainWindow::stopClicked,
                     controller,
                     &AbstractController::dontListen
                     );


    w.listenChanged(false);
    w.show();

    int result = a.exec();
    delete controller;
  //  Options::getInstance()->save();
    return result;
}

