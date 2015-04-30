#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stdafx.h"
#include <QObject>
#include <QThread>
#include "receiver.h"
#include "noisegen.hpp"
#include "fftransformer.h"
#include <QVector>
#include "abstractcontroller.h"

class Controller : public AbstractController
{
private:
    Q_OBJECT
    Receiver* m_rx;
    FFTransformer* m_fft;
    Scheduler* m_scheduler;
    FFTJobManager m_workQueue;

public:
    Controller():
        m_rx(nullptr),
        m_fft(nullptr),
        m_scheduler(nullptr)
    {
    }

    virtual ~Controller()
    {
        stop();
    }

    bool working()const { return m_rx != nullptr; }

public slots:
    virtual void stop()
    {
        if(m_fft != nullptr)
        {
            m_fft->setDataSource(nullptr);
            delete m_fft;
            m_fft = nullptr;
        }

        if(m_rx != nullptr)
        {
            delete m_rx;
            m_rx = nullptr;
        }

        if(m_scheduler != nullptr)
        {
            delete m_scheduler;
            m_scheduler = nullptr;
        }
    }

    virtual void dontListen()
    {
        if(m_scheduler != nullptr)
        {
            delete m_scheduler;
            m_scheduler = nullptr;
            emit listenChanged(false);
        }
    }

    virtual void listen(const QTime& when)
    {
        if(m_scheduler == nullptr)
        {
            m_scheduler = new Scheduler(this, when);
            QObject::connect(m_scheduler, &Scheduler::launch, this, &Controller::launch);
            emit listenChanged(true);
        }
        else
            qDebug() << "already listening on " << m_scheduler->when().toString("hh:mm:ss:zzz");;
    }

    void launch()
    {
        if(working())
        {
            showWarning(nullptr, "Ошибка запуска", "Анализ уже запущен.");
            return;
        }
        else if (m_scheduler == nullptr)
        {
            showWarning(nullptr, "Внимание", "Внеплановый запуск.");
            listen(QTime::currentTime());
            return;
        }

        try
        {
            qDebug() << "launching at " << QTime::currentTime().toString("hh:mm:ss:zzz");

            m_rx = new Receiver(this, m_scheduler->when(), &m_workQueue);
            QObject::connect(m_rx, &Receiver::done, this, &Controller::done);

            m_fft = new FFTransformer(this, Options::getInstance()->getFFTThreads());
            QObject::connect(m_fft, &FFTransformer::dataProcessed, this, &Controller::dataProcessed);
            m_fft->start();
            m_fft->setDataSource(&m_workQueue);
        }
        catch(QString& message)
        {
            showWarning(nullptr, "Ошибка запуска", message);
        }
    }
public slots:
    void done()
    {
        if(m_rx != nullptr)
        {
            delete m_rx;
            m_rx = nullptr;
            emit listenChanged(false);
        }
    }
};
#endif // CONTROLLER_H
