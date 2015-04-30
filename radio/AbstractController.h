#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H
#include <QObject>
#include "scheduler.h"

class AbstractController : public QObject
{
    Q_OBJECT
public:
    virtual bool working()const = 0;

public slots:
    virtual void launch() = 0;
    virtual void done() = 0;
    virtual void stop() = 0;
    virtual void listen(const QTime& when) = 0;
    virtual void dontListen() = 0;
signals:
    void dataProcessed(double* data, size_t column, size_t length);
    void listenChanged(bool value);
};

#endif // ABSTRACTCONTROLLER_H
