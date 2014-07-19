#include "workerfft.h"
#include <QVector>

WorkerFFT::WorkerFFT(QQueue<Complex>* queue, QThread* thread, size_t bufferSize, size_t bufferOverlap) :
    m_bufferSize(bufferSize),
    m_bufferOverlap(bufferOverlap),
    m_plan(nullptr),
    Worker(thread),
    m_accessMutex(new QMutex()),
    m_data(queue)
{
    if(m_bufferSize <= m_bufferOverlap)
    {
        qWarning("Overlap is bigger than buffer size; setting overlap to 0!");
        m_bufferOverlap = 0;
    }

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

    qDebug("FFTWorker started with buffer size = %s and overlap = %s",
            qPrintable(QString::number(m_bufferSize)),
            qPrintable(QString::number(m_bufferOverlap)));

    while(getActive())
    {
        m_accessMutex->lock();
        if(m_data->length() < m_bufferSize)
        {
            m_accessMutex->unlock();
            getThread()->msleep(100);
            continue;
        }

        for(auto i = 0; i != m_bufferSize - m_bufferOverlap; i++)
        {
            Complex c = m_data->dequeue();           
            *(m_inBuffer + i)[0] = c.real();
            *(m_inBuffer + i)[1] = c.imag();
        }

        for(auto i = 0; i != m_bufferOverlap; i++)
        {
            Complex c = m_data->at(i);
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
