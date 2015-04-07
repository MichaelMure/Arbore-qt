#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T23:09:50
#
#-------------------------------------------------

QT       += core gui network script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Arbore-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ipfs.cpp

HEADERS  += mainwindow.h \
    ipfs.h

FORMS    += mainwindow.ui
