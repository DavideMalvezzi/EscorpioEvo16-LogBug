#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T14:22:21
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LogBug
TEMPLATE = app
RC_FILE = app.rc


SOURCES += main.cpp\
        mainwindow.cpp\
        logmodel.cpp \
    logrow.cpp \
    serialreader.cpp \
    logfiltermodel.cpp

HEADERS  += mainwindow.h\
            logmodel.h \
    logrow.h \
    serialreader.h \
    logfiltermodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    assets/assets.qrc

DISTFILES +=
