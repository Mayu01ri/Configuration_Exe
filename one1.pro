#-------------------------------------------------
#
# Project created by QtCreator 2018-12-20T17:56:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = one
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    uart.h \
    mythread.h

FORMS    += mainwindow.ui
