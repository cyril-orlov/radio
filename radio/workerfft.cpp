#include "workerfft.h"
#include <QVector>
#include <QFile>

WorkerFFT::WorkerFFT(QThread* thread, size_t bufferSize, size_t bufferStep) :
    m_bufferSize(bufferSize),
    m_bufferStep(bufferStep),
    m_plan(nullptr),
    m_dataSource(nullptr),
    m_safeExit(false),
    Worker(thread)
{
    if(m_bufferSize <= m_bufferStep)
    {
        qWarning("Step is bigger than buffer size; setting step to 1!");
        m_bufferStep = 1;
    }

    setObjectName(QString("FFT"));
}

void WorkerFFT::init(fftw_complex* complexSub)
{
    m_sampleBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_spectrumBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_complexSub = complexSub;

    m_plan = fftw_plan_dft_1d(m_bufferSize, m_sampleBuffer, m_spectrumBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
    m_inversePlan = fftw_plan_dft_1d(m_bufferSize, m_spectrumBuffer, m_sampleBuffer, FFTW_BACKWARD, FFTW_ESTIMATE);
}

void WorkerFFT::setSafeExit()
{
    m_safeExit = true;
}

void WorkerFFT::work()
{
    qDebug("FFTWorker started with buffer size = %s and step = %s",
            qPrintable(QString::number(m_bufferSize)),
            qPrintable(QString::number(m_bufferStep)));

#define exit_or_wait \
    if(!m_safeExit)\
    {\
        QThread::msleep(10);\
        continue;\
    }\
    else\
    {\
        emit finished();\
        break;\
    }

    while(getActive())
    {
        if(!m_dataSource)
        {
            exit_or_wait
        }

        m_dataSource->lock();

        if(!m_dataSource->any())
        {
            m_dataSource->unlock();

            exit_or_wait
        }

        auto job = m_dataSource->dequeue();

        m_dataSource->unlock();

        if(job != nullptr)
            handleJob(job);
    }
#undef exit_or_wait
}

void WorkerFFT::setDataSource(FFTJobManager* dataSource)
{
    m_dataSource = dataSource;
}

void WorkerFFT::handleJob(FFTJob<Complex>* job)
{
    auto jobBuffer = job->getBuffer();
    size_t steps = (job->length() - m_bufferSize) / m_bufferStep + 1;
    double* buffer = new double[steps];

#ifdef DUMP_RAW
    DumpRaw(job);
#endif

    for (size_t i = 0; i < steps; i++)
    {
        // forward
        for (int j = 0; j < m_bufferSize; ++j)
        {
            auto iter = m_sampleBuffer[j];
            Q_ASSERT(j + i * m_bufferStep < job->length());
            iter[0] = jobBuffer[j + i * m_bufferStep].real();
            iter[1] = jobBuffer[j + i * m_bufferStep].imag();
        }

        fftw_execute(m_plan);

        // convolute
        for(size_t j = 0; j < m_bufferSize; j++)
        {
            auto i1 = *(m_spectrumBuffer + j);
            auto i2 = *(m_complexSub + j);
            auto re = i1[0] * i2[0] - i1[1] * i2[1];
            auto im = i1[1] * i2[0] + i1[0] * i2[1];
            i1[0] = re;
            i1[1] = im;
        }

        // backward
        fftw_execute(m_inversePlan);

        auto first = *m_sampleBuffer;
        buffer[i] = log10((first[0] * first[0] + first[1] * first[1]) / m_bufferSize);
    }

    auto result = new FilterResult(buffer, steps, job->frequency());
#ifdef DUMP_FFT
    DumpFFT(result);
#endif

    emit digested(result);
}

#ifdef DUMP_FFT
void WorkerFFT::DumpFFT(FilterResult* job)
{
    QFile file(QString("dumpfft-" + QString::number(job->frequency(), 'f', 3) + "mhz.log"));
    if(!file.open(QIODevice::Append | QIODevice::WriteOnly))
        qDebug("unable to dump fft");
    else
    {
        auto b = job->getBuffer();
        QTextStream stream(&file);
        for(size_t i = 0; i < job->length(); i++)
        {
            stream << b[i];
            endl(stream);
        }
        file.close();
    }
}
#endif

#ifdef DUMP_RAW
void WorkerFFT::DumpRaw(FFTJob<Complex> *job)
{
    QFile file(QString("dumpraw-" + QString::number(job->frequency(), 'f', 3) + "mhz.log"));
    if(!file.open(QIODevice::Append | QIODevice::WriteOnly))
        qDebug("unable to dump raw samples");
    else
    {
        auto b = job->getBuffer();
        QTextStream stream(&file);
        for(size_t i = 0; i < job->length(); i++)
        {
            stream << b[i].real() << " " << b[i].imag();
            endl(stream);
        }
        file.close();
    }
}
#endif

void WorkerFFT::afterDestroy()
{
    fftw_destroy_plan(m_plan);    
    fftw_destroy_plan(m_inversePlan);
    fftw_free(m_sampleBuffer);
    fftw_free(m_spectrumBuffer);
}
