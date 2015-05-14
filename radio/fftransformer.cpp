#include "fftransformer.h"
#include "fftw3.h"

FFTransformer::FFTransformer(QObject *parent, char workersCount):
    QObject(parent),
    m_workersCount(workersCount),
    m_workersStopped(0),
    m_complexSub(nullptr)
{
    if(!fftw_init_threads())
        throw QString("unable to init fftw threading!");

    auto o = Options::getInstance();

    m_bufferSize = o->getBand()
            * Options::calculateActualBand(o->getActualBand(), o->getSignalSpeed())
            / o->getSignalSpeed();

    size_t i = 2;
    while ((i *= 2) < m_bufferSize) ;
    m_bufferSize = i;

    size_t bufferOverlap = (size_t)(o->getBand() / o->getActualBand());

    if(workersCount <= 0)
        workersCount = 1;
    m_threads = new QThread*[workersCount];
    m_workers = new WorkerFFT*[workersCount];

    for (int i = 0; i < workersCount; ++i)
    {
        m_threads[i] = new QThread(this);
        m_workers[i] = new WorkerFFT(m_threads[i], m_bufferSize, bufferOverlap);
        connect(m_workers[i], &WorkerFFT::digested, this, &FFTransformer::dataProcessed);        
        connect(m_workers[i], &WorkerFFT::finished, this, &FFTransformer::workerFinished);
    }
}

void FFTransformer::setDataSource(FFTJobManager* dataSource)
{
    for (int i = 0; i < m_workersCount; ++i)
        m_workers[i]->setDataSource(dataSource);
}


void FFTransformer::setSafeExit()
{
    for (int i = 0; i < m_workersCount; ++i)
        m_workers[i]->setSafeExit();
}

void FFTransformer::start()
{
    m_complexSub = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    fillComplexSub();
    for (int i = 0; i < m_workersCount; ++i)
    {
        m_workers[i]->init(m_complexSub);
        m_threads[i]->start();
    }
}

void FFTransformer::fillComplexSub()
{
    auto plan = fftw_plan_dft_1d(m_bufferSize, m_complexSub, m_complexSub, FFTW_FORWARD, FFTW_ESTIMATE);
    double speed = Options::getInstance()->getSignalSpeed();
    double rxRate = Options::getInstance()->getBand();
    for(size_t i = 0; i < m_bufferSize; i++)
    {
        double t = i / rxRate;
        t *= t;

        auto iter = *(m_complexSub + i);
        iter[0] = cos(M_PI * t * speed);
        iter[1] = -sin(M_PI * t * speed);
    }

    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

void FFTransformer::workerFinished()
{
    if(++m_workersStopped == m_workersCount)
        emit finished();
}

FFTransformer::~FFTransformer()
{
    if(m_complexSub != nullptr)
    {
        fftw_free(m_complexSub);
        for (int i = 0; i < m_workersCount; ++i)
            delete m_workers[i];
    }
}
