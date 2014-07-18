#include "workerfft.h"
#include <QVector>

WorkerFFT::WorkerFFT(QQueue<Complex>* queue, QThread* thread, size_t bufferSize) :
    m_bufferSize(bufferSize),
    m_plan(nullptr),
    Worker(thread),
    m_accessMutex(new QMutex()),
    m_data(queue)
{
    setObjectName(QString("FFT"));
}

void WorkerFFT::init()
{
    m_inBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_outBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_plan = fftw_plan_dft_1d(m_bufferSize, m_inBuffer, m_outBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
}

QMutex* WorkerFFT::mutex()
{
    return m_accessMutex;
}

void WorkerFFT::work()
{
    init();

    qDebug("FFTWorker started with buffer size = %s", qPrintable(QString::number(m_bufferSize)));
    while(getActive())
    {
        m_accessMutex->lock();
        if(m_data->length() < m_bufferSize)
        {
            m_accessMutex->unlock();
            getThread()->msleep(100);
            continue;
        }

        for(auto i = 0; i != m_bufferSize; i++)
        {
            Complex c = m_data->dequeue();           
            *(m_inBuffer + i)[0] = c.real();
            *(m_inBuffer + i)[1] = c.imag();
        }
        m_accessMutex->unlock();

        fftw_execute(m_plan);

        QVector<double> transformed;
        for (auto i = 0; i < m_bufferSize; ++i)
        {
            fftw_complex* iter = (m_outBuffer + i);
            double re = *(iter)[0];
            double im = *(iter)[1];
            transformed.append((re * re + im * im) / m_bufferSize);
        }

        emit digested(transformed);
    }
}

WorkerFFT::~WorkerFFT()
{
    fftw_destroy_plan(m_plan);    
    fftw_free(m_inBuffer);
    fftw_free(m_outBuffer);
}
