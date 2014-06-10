#include "optionsdialog.h"
#include "ui_optionsDialog.h"
#include "options.h"
#include <QMessageBox>

OptionsDialog::OptionsDialog():
    QDialog(),
    ui(new Ui::OptionsDialog())
{
    ui->setupUi(this);
}


void showWarning(QWidget * parent, const QString &title, const QString &text);


void OptionsDialog::on_OptionsDialog_accepted()
{
    int timeLeft = ui->timeEdit->value();
    int band = ui->bandEdit->value();
    double frequency = ui->frequencyEdit->value();
    QString address = ui->addressEdit->text();

    Options* options = Options::getInstance();
    options->setTimeLeft(timeLeft);
    options->setBand(band);
    options->setFrequency(frequency * 1000);
    if(!options->setAddress(address))
        showWarning(this, QString("Ошибка"), QString("Некорректный адрес устройства"));

    emit optionsUpdated();
}

int OptionsDialog::exec()
{
    Options* options = Options::getInstance();
    ui->timeEdit->setValue(options->getTimeLeft());
    ui->bandEdit->setValue(options->getBand());
    ui->frequencyEdit->setValue(options->getFrequency() / 1000);
    ui->addressEdit->setText(options->getAddress().toString());
    return QDialog::exec();
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void showWarning(QWidget * parent, const QString &title, const QString &text)
{
    QMessageBox box(parent);
    box.setIcon(QMessageBox::Icon::Warning);
    box.setWindowTitle(title);
    box.setText(text);
    QSpacerItem* spacer = new QSpacerItem(300, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)box.layout();
    layout->addItem(spacer, layout->rowCount(), 0, 1, layout->columnCount());
    box.exec();
}

#undef CHECK_VALID
