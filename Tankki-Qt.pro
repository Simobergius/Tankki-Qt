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
        devicebutton.cpp \
        bluetoothworker.cpp \
        math_utils.cpp \
        touchareawidget.cpp \
        settingsdialog.cpp

HEADERS  += mainwindow.h \
        paireddeviceswindow.h \
        devicebutton.h \
        bluetoothworker.h \
        math_utils.h \
        touchareawidget.h \
        settingsdialog.h

FORMS    += mainwindow.ui \
        paireddeviceswindow.ui \
    settingsdialog.ui
