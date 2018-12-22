#-------------------------------------------------
#
# Project created by QtCreator 2018-09-21T17:48:26
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tankki-Qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        paireddeviceswindow.cpp \
        devicebutton.cpp

HEADERS  += mainwindow.h \
        paireddeviceswindow.h \
        devicebutton.h

FORMS    += mainwindow.ui \
        paireddeviceswindow.ui
