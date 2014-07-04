#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T10:35:13
#
#-------------------------------------------------

include(../qwt-6.1.0/qwt.prf)

INCLUDEPATH += ../qwt-6.1.0/src \
            ../uhd/include \
            ../boost_1_55_0 \
            ../fftw-3.3.4-dll32

DEPENDPATH += ../qwt-6.1.0/lib \
              ../fftw-3.3.4-dll32

UHDPATH = ../uhd/lib

CONFIG(Debug, Debug|Release) {
    UHDPATH = ../uhd/lib
    LIBS += -L../uhd/lib -luhd\
}

CONFIG(Release, Debug|Release) {
    UHDPATH = ../uhd_release/lib
    LIBS += -L../uhd_release/lib -luhd\
}

DEPENDPATH += UHDPATH

LIBS += -L../fftw-3.3.4-dll32 -llibfftw3-3

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = radio
TEMPLATE = app


SOURCES += main.cpp\
    optionsdialog.cpp \
    mainwindow.cpp \
    options.cpp \
    timer.cpp \
    receiver.cpp \
    workerrx.cpp \
    workertime.cpp \
    fftransformer.cpp

HEADERS  += mainwindow.h \
    options.h \
    optionsdialog.h \
    timer.h \
    receiver.h \
    workerrx.h \
    workertime.h \
    worker.hpp \
    fftransformer.h \
    datahelper.hpp

FORMS    += mainwindow.ui \
    optionsDialog.ui

