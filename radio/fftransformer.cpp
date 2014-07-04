#include "fftransformer.h"
#include "fftw3.h"

FFTransformer::FFTransformer(size_t bufferSize, MainWindow *parent) :
    QObject(parent),
    m_bufferSize(bufferSize),
    m_plan(nullptr)
{
    init();
}

void FFTransformer::init()
{
    if(m_plan != nullptr)
        fftw_destroy_plan(m_plan);

    m_inBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_outBuffer = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * m_bufferSize);
    m_plan = fftw_plan_dft_1d(m_bufferSize, m_inBuffer, m_outBuffer, FFTW_FORWARD, FFTW_ESTIMATE);
}

void FFTransformer::onDataReceived(Complex* data, size_t count)
{
    if(count > m_bufferSize)
    {
        qDebug("data size is bigger than buffer size (FFTransformer)");
        return;
    }

    memcpy(m_inBuffer, data, count * sizeof(Complex));

    fftw_execute(m_plan);

    QVector<Complex>* transformed = new QVector<Complex>;
    for (auto i = 0; i < count; ++i)
    {
        fftw_complex* iter = (m_outBuffer + i);
        transformed->append(Complex(*(iter)[0], *(iter)[1]));
    }

    ((MainWindow*)parent())->onChartChanged(transformed);
}

FFTransformer::~FFTransformer()
{
    fftw_destroy_plan(m_plan);
    fftw_free(m_inBuffer);
    fftw_free(m_outBuffer);
}
