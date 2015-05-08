#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T23:09:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Arbore-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ipfs/ipfs.cpp \
    ipfs/ipfsversion.cpp \
    ipfs/ipfspin.cpp \
    ipfs/ipfsswarm.cpp \
    download.cpp \
    downloadmodel.cpp \
    downloaddelegate.cpp \
    ipfs/ipfsid.cpp \
    ipfs/ipfspeer.cpp \
    ipfs/ipfsget.cpp

HEADERS  += mainwindow.h \
    ipfs/ipfs.h \
    ipfs/ipfsversion.h \
    ipfs/ipfspin.h \
    ipfs/ipfshash.h \
    ipfs/ipfsswarm.h \
    ipfs/ipfspeer.h \
    download.h \
    downloadmodel.h \
    downloaddelegate.h \
    ipfs/ipfsid.h \
    ipfs/abstractipfscommand.h \
    ipfs/ipfsget.h

FORMS    += mainwindow.ui
