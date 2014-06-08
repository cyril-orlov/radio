#-------------------------------------------------
#
# Project created by QtCreator 2014-06-06T10:35:13
#
#-------------------------------------------------

include(../../radio/qwt-6.1.0/qwt.prf)

INCLUDEPATH += ../../radio/qwt-6.1.0/src

DEPENDPATH += ../../radio/qwt-6.1.0/lib

QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = radio
TEMPLATE = app


SOURCES += main.cpp\
    optionsdialog.cpp \
    mainwindow.cpp \
    options.cpp \
    timer.cpp \
    worker.cpp

HEADERS  += mainwindow.h \
    options.h \
    optionsdialog.h \
    timer.h \
    worker.h

FORMS    += mainwindow.ui \
    optionsDialog.ui
