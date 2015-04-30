#include "listendialog.h"
#include "ui_listendialog.h"

ListenDialog::ListenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListenDialog)
{
    ui->setupUi(this);
}

int ListenDialog::exec()
{
    ui->timeEdit->setTime(QTime::currentTime());
    auto result = QDialog::exec();
    m_time = ui->timeEdit->time();
    return result;
}

const QTime& ListenDialog::getTime()const
{
    return m_time;
}

ListenDialog::~ListenDialog()
{
    delete ui;
}
