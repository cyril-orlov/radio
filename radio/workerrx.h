#ifndef WORKERRX_H
#define WORKERRX_H

#include "stdafx.h"
#include "worker.hpp"
#include <QObject>
#include <complex>
#include <QVector>
#include "uhd/usrp/multi_usrp.hpp"
#include "jobmanager.hpp"

void showWarning(QWidget * parent, const QString &title, const QString &text);

typedef std::complex<T_REAL> Complex;
typedef JobManager<FFTJob<Complex>*> FFTJobManager;
typedef Complex *Samples;

class WorkerRx : public Worker
{
    Q_OBJECT

public:
    struct Config
    {       
        uhd::usrp::multi_usrp::sptr device;
        uhd::rx_streamer::sptr stream;

        double startFrequency, endFrequency, signalSpeed;
        double actualBand;

        // output data structure
        FFTJobManager* buffer;

        // how many samples to request per band
        size_t samples;

        // how long to wait on uhd stream to deliver samples
        double timeout;

        // seconds to wait before first rx
        QTime when;

        size_t extraTicks;
    };

private:
    Config m_config;

    uhd::time_spec_t doFirstRx();
    uhd::time_spec_t doRx(double freq, double sec, const uhd::time_spec_t &time);
    bool checkMetadataError(const uhd::rx_metadata_t& metadata);   
    void WorkerRx::timeFix();

#ifdef DUMP_RAW
    void Dump(Complex* buffer);
#endif
public:
    static const int SAMPLE_SIZE = sizeof(Complex);
    WorkerRx(QThread* thread, const Config& cfg);
signals:
    void dataReceived(FFTJob<Complex>* data);
    void error(const QString & message);
    void done();

public slots:
    void work() override;
};

#endif // WORKERRX_H
