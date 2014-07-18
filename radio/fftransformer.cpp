#include "fftransformer.h"
#include "fftw3.h"

FFTransformer::FFTransformer(size_t bufferSize) :
    m_data(QQueue<Complex>()),
    m_thread(new QThread())
{
    m_worker = new WorkerFFT(&m_data, m_thread, bufferSize);
    this->connect(m_worker, &WorkerFFT::digested, this, &FFTransformer::onDataProcessed);
    m_thread->start();
}

void FFTransformer::onDataReceived(Complex* data, size_t count)
{
    m_worker->mutex()->lock();
    for (auto i = 0; i < count; ++i)
        m_data.enqueue(*(data + i));
    m_worker->mutex()->unlock();

    delete data;
}

void FFTransformer::onDataProcessed(QVector<double> &data)
{
    emit dataProcessed(data);
}

FFTransformer::~FFTransformer()
{
    if (m_worker)
    {
        m_worker->deactivate();
        delete m_worker;
    }
    if (m_thread)
        delete m_thread;
}
