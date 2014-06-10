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

void FFTransformer::onDataReceived(QVector<Complex> * data)
{
    if(data->size() > m_bufferSize)
    {
        qDebug("data size is bigger than buffer size (FFTRansformer)");
        return;
    }

    size_t size = data->size();
    for (auto i = 0; i < size; ++i)
    {
        fftw_complex c = { data->at(i).real(), data->at(i).imag() };
        m_inBuffer[i][0] = c[0];
        m_inBuffer[i][1] = c[1];
    }

    fftw_execute(m_plan);

    data->clear();
    for (auto i = 0; i < size; ++i)
    {
        fftw_complex* iter = (m_outBuffer + i);
        data->append(Complex(*(iter)[0], *(iter)[1]));
    }

    ((MainWindow*)parent())->onChartChanged(data);
}

FFTransformer::~FFTransformer()
{
    fftw_destroy_plan(m_plan);
    fftw_free(m_inBuffer);
    fftw_free(m_outBuffer);
}
