#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T10:35:13
#
#-------------------------------------------------

include(../qwt-6.1.0/qwt.prf)

INCLUDEPATH += ../qwt-6.1.0/src \
            ../uhd/include \
            ../boost_1_55_0

DEPENDPATH += ../qwt-6.1.0/lib \
            ../uhd/lib

LIBS += -L../uhd/lib -luhd

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = radio
TEMPLATE = app


SOURCES += main.cpp\
    optionsdialog.cpp \
    mainwindow.cpp \
    options.cpp \
    timer.cpp \
    worker.cpp \
    receiver.cpp

HEADERS  += mainwindow.h \
    options.h \
    optionsdialog.h \
    timer.h \
    worker.h \
    receiver.h

FORMS    += mainwindow.ui \
    optionsDialog.ui

