// generates random samples
// uses Park-Miller RNG

#include <numeric>
#include <complex>
#include <QThread>
#include <QMutex>
#include <QVector>

typedef std::complex<double> Complex;

class NoiseGen : public QObject
{
    Q_OBJECT

private:
    long m_seed;
    Complex *m_buffer;
    size_t m_bufferSize;
    QMutex* m_bufferLock;
    int m_sleepTime, m_sleepPeriod, m_currentSleepStage;
    bool m_active;

public:
    NoiseGen(int sleepTime = 1, int sleepPeriod = 5, long seed = 1, size_t bufferSize = 1 << 10)
        : m_buffer(new Complex[bufferSize]),
          m_bufferSize(bufferSize),
          m_seed(seed),
          m_bufferLock(new QMutex()),
          m_sleepTime(sleepTime),
          m_sleepPeriod(sleepPeriod),
          m_active(true),
          m_currentSleepStage(0)
    {
    }

    void deactivate()
    {
        m_active = false;
    }

    Complex next()
    {
        double max = std::numeric_limits<long>::max();
        double re = nextInt() / max, im = nextInt() / max;
        return Complex(re, im);
    }

    ~NoiseGen()
    {
        qDebug("NoiseGen destroyed");
        delete m_buffer;
        delete m_bufferLock;
    }

public slots:
    void onStart()
    {
        qDebug("NoiseGen started with buffer size = %s", qPrintable(QString::number(m_bufferSize)));
        while(m_active)
        {
            static int bufferIndex = 0;
            if(bufferIndex == m_bufferSize)
            {
                emitReceived();
                bufferIndex = 0;
            }

            m_bufferLock->lock();
            m_buffer[bufferIndex++] = next();
            m_bufferLock->unlock();

            if(m_currentSleepStage == m_sleepPeriod)
            {
                m_currentSleepStage = 0;
                QThread::msleep(m_sleepTime);
            }
            else
                m_currentSleepStage++;
        }
    }

signals:
    void received(Complex *data, size_t count);

private:
    void emitReceived()
    {
        m_bufferLock->lock();

        Complex* data = new Complex[m_bufferSize];
        memcpy(data, m_buffer, sizeof(Complex) * m_bufferSize);
        emit received(data, m_bufferSize);
        qDebug("Noise emitted");

        m_bufferLock->unlock();
    }

    unsigned long nextInt()
    {
        unsigned long k;

        k = m_seed / 12773;
        m_seed = 16807 * (m_seed - k * 12773) - 2836 * k;

        return m_seed;
    }
};
