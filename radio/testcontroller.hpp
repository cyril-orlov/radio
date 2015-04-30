#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include "stdafx.h"
#include <QObject>
#include <QThread>
#include "receiver.h"
#include "noisegen.hpp"
#include "fftransformer.h"
#include <QVector>
#include "abstractcontroller.h"

class TestController : public AbstractController
{
private:
    Q_OBJECT

    QThread* m_ngthread;
    NoiseGen* m_ng;

    FFTransformer* m_fft;
public:
    TestController()
    {
        m_ng = nullptr;
        m_ngthread = nullptr;
        m_fft = new FFTransformer(this, 1);//Options::getInstance()->getFFTThreads());
        QObject::connect(m_fft, &FFTransformer::dataProcessed, this, &TestController::dataProcessed);
        m_fft->start();
    }

    bool working() const
    {
        return false;
    }

public slots:
    virtual void stop()
    {
        m_fft->setDataSource(nullptr);

        if(m_ng != nullptr)
        {
            m_ng->deactivate();
            m_ngthread->wait();
            delete m_ng;
        }

        delete m_fft;
    }

    void launch()
    {
        if(m_ngthread != nullptr)
        {
            m_ng->deactivate();
            m_ngthread->wait();
            delete m_ngthread;
        }

        m_ng = new NoiseGen(363, QString("test550-1.dat"));
        m_ngthread = new QThread(this);
        m_ng->moveToThread(m_ngthread);
        QObject::connect(m_ngthread, &QThread::started, m_ng, &NoiseGen::onStart);
        m_ngthread->start();

        m_fft->setDataSource(m_ng->getDataSource());
    }

    void listen(const QTime& when)
    {
        launch();
    }

    void dontListen()
    {
    }

    void done()
    {

    }
};
#endif // TESTCONTROLLER_H
