#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T23:09:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Arbore-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ipfs/ipfs.cpp \
    ipfs/ipfsversion.cpp \
    ipfs/ipfspin.cpp \
    ipfs/ipfsswarm.cpp \
    ipfs/ipfsid.cpp \
    ipfs/ipfspeer.cpp \
    ipfs/ipfsget.cpp \
    share.cpp \
    sharedelegate.cpp \
    sharemodel.cpp \
    ipfs/ipfsls.cpp \
    ipfs/ipfshash.cpp \
    directory.cpp \
    file.cpp \
    object.cpp \
    ipfs/ipfsrefs.cpp \
    ipfs/ipfsstats.cpp

HEADERS  += mainwindow.h \
    ipfs/ipfs.h \
    ipfs/ipfsversion.h \
    ipfs/ipfspin.h \
    ipfs/ipfshash.h \
    ipfs/ipfsswarm.h \
    ipfs/ipfspeer.h \
    ipfs/ipfsid.h \
    ipfs/ipfsget.h \
    ipfs/iapilistener.h \
    ipfs/ipfsls.h \
    directory.h \
    file.h \
    object.h \
    share.h \
    sharedelegate.h \
    sharemodel.h \
    ipfs/ipfsrefs.h \
    ipfs/ipfsstats.h

FORMS    += mainwindow.ui
