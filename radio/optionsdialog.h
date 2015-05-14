#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include "stdafx.h"
#include <QDialog>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    OptionsDialog();
    ~OptionsDialog();

public slots:
    int exec() override;

private slots:
    void on_OptionsDialog_accepted();

    void onRecalculateBand();

private:
    Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
