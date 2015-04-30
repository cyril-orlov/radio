#ifndef LISTENDIALOG_H
#define LISTENDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui {
class ListenDialog;
}

class ListenDialog : public QDialog
{
    Q_OBJECT
    QTime m_time;
public:
    explicit ListenDialog(QWidget *parent = 0);
    ~ListenDialog();
    int exec() override;
    const QTime& getTime()const;

private:
    Ui::ListenDialog *ui;
};

#endif // LISTENDIALOG_H
