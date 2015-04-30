#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T10:35:13
#
#-------------------------------------------------

include(../qwt-6.1.0/qwt.prf)
QMAKE_CXXFLAGS += /wd4996
INCLUDEPATH += ../qwt-6.1.0/src \
            ../uhd/include \
            ../boost_1_55_0 \
            ../fftw-3.3.4-dll32

INCLUDEPATH += $$PWD/../boost_1_55_0/stage
DEPENDPATH += $$PWD/../boost_1_55_0/stage

DEPENDPATH += ../qwt-6.1.0/lib \
              ../fftw-3.3.4-dll32 \
              ../boost_1_55_0/stage

UHDPATH = ../uhd/lib

CONFIG(Debug, Debug|Release) {
    UHDPATH = ../uhd/lib
    LIBS += -L../uhd/lib -luhd
    LIBS += -L$$PWD/../boost_1_55_0/stage/lib/ -llibboost_program_options-vc100-mt-gd-1_55
}

CONFIG(Release, Debug|Release) {
    UHDPATH = ../uhd_release/lib
    LIBS += -L../uhd_release/lib -luhd
    LIBS += -L$$PWD/../boost_1_55_0/stage/lib/ -llibboost_program_options-vc100-mt-1_55
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
    receiver.cpp \
    workerrx.cpp \
    fftransformer.cpp \
    workerfft.cpp \
    datahelper.cpp \
    scheduler.cpp \
    listendialog.cpp

HEADERS  += mainwindow.h \
    options.h \
    optionsdialog.h \
    receiver.h \
    workerrx.h \
    worker.hpp \
    fftransformer.h \
    workerfft.h \
    noisegen.hpp \
    customplot.hpp \
    datahelper.h \
    controller.hpp \
    jobmanager.hpp \
    stdafx.h \
    abstractcontroller.h \
    scheduler.h \
    listendialog.h \
    testcontroller.hpp

FORMS    += mainwindow.ui \
    optionsDialog.ui \
    listendialog.ui
