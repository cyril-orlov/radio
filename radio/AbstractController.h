#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H
#include <QObject>
#include "scheduler.h"
#include "jobmanager.hpp"

class AbstractController : public QObject
{
    Q_OBJECT
public:
    virtual bool working()const = 0;

public slots:
    virtual void launch() = 0;
    virtual void stop() = 0;
    virtual void listen(const QTime& when) = 0;
    virtual void dontListen() = 0;
signals:
    void dataProcessed(FilterResult* data);
    void listenChanged(bool value);
};

#endif // ABSTRACTCONTROLLER_H
