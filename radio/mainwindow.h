#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <complex>

namespace Ui {
class MainWindow;
}

typedef std::complex<double> Complex;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void optionsClicked_internal();

signals:
    void optionsClicked();

public slots:
    void timerDone();
    void timerUpdate(int count);
    void onOptionsChanged();
    void onChartChanged(QVector<Complex> * data);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
