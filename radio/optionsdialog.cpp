#include "optionsdialog.h"
#include "ui_optionsDialog.h"
#include "options.h"

OptionsDialog::OptionsDialog():
    QDialog(),
    ui(new Ui::OptionsDialog())
{
    ui->setupUi(this);
}

void OptionsDialog::on_OptionsDialog_accepted()
{
    int timeLeft = ui->timeEdit->value();
    int band = ui->bandEdit->value();
    int frequency = ui->frequencyEdit->value();

    Options* options = Options::getInstance();
    options->setTimeLeft(timeLeft);
    options->setBand(band);
    options->setFrequency(frequency);

    emit optionsUpdated();
}

int OptionsDialog::exec()
{
    Options* options = Options::getInstance();
    ui->timeEdit->setValue(options->getTimeLeft());
    ui->bandEdit->setValue(options->getBand());
    ui->frequencyEdit->setValue(options->getFrequency());
    return QDialog::exec();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

#undef CHECK_VALID
