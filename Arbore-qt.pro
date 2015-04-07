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
    ipfs/ipfs.cpp \
    ipfs/ipfsversion.cpp \
    ipfs/ipfspin.cpp

HEADERS  += mainwindow.h \
    ipfs/ipfs.h \
    ipfs/ipfscommand.h \
    ipfs/ipfsversion.h \
    ipfs/ipfspin.h \
    ipfs/ipfshash.h

FORMS    += mainwindow.ui
