#ifndef WORKERRX_H
#define WORKERRX_H

#include "worker.hpp"
#include <QObject>
#include <complex>
#include <QVector>
#include "uhd/usrp/multi_usrp.hpp"

void showWarning(QWidget * parent, const QString &title, const QString &text);

typedef std::complex<double> Complex;
typedef QVector<Complex> Samples;

class WorkerRx : public Worker
{
    Q_OBJECT

public:
    struct Config
    {
        long frequency;
        uhd::usrp::multi_usrp::sptr device;
        uhd::rx_streamer::sptr stream;
        uhd::time_spec_t time;
        double timeout;
    };

private:
    Config m_config;

    void setCountdown(size_t bufferSize);
    bool checkMetadataError(const uhd::rx_metadata_t& metadata);

public:
    static const int SAMPLE_SIZE = sizeof(Complex);

    explicit WorkerRx(const Config& config, QThread *thread = 0);

signals:
    void dataReceived(Samples* data);
    void error(const QString & message);

public slots:
    void work() override;
};

#endif // WORKERRX_H
