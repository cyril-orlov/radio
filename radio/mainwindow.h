#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"
#include "customplot.hpp"
#include <QMainWindow>
#include <QVector>
#include <complex>
#include <QTime>
#include "listendialog.h"
#include "scheduler.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    CustomPlot * qwtPlot;
    ~MainWindow();

private slots:
    void optionsClicked_internal();
    void startClicked_internal();
    void stopClicked_internal();
signals:
    void optionsClicked();
    void startClicked(const QTime& when);
    void stopClicked();

public slots:
    void onChartChanged(FilterResult *data);   
    void listenChanged(bool value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
