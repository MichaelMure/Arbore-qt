#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T23:09:50
#
#-------------------------------------------------

QT       += core network qml quick

CONFIG += c++11
CONFIG += debug_and_release

TARGET = Arbore-qt
TEMPLATE = app


SOURCES += main.cpp\
    ipfs/ipfs.cpp \
    ipfs/ipfsversion.cpp \
    ipfs/ipfspin.cpp \
    ipfs/ipfsswarm.cpp \
    ipfs/ipfsid.cpp \
    ipfs/ipfspeer.cpp \
    ipfs/ipfsget.cpp \
    share.cpp \
    sharemodel.cpp \
    ipfs/ipfsls.cpp \
    ipfs/ipfshash.cpp \
    directory.cpp \
    file.cpp \
    object.cpp \
    ipfs/ipfsrefs.cpp \
    ipfs/ipfsstats.cpp \
    objectiterator.cpp

HEADERS  += \
    ipfs/ipfs.h \
    ipfs/ipfsversion.h \
    ipfs/ipfspin.h \
    ipfs/ipfshash.h \
    ipfs/ipfsswarm.h \
    ipfs/ipfspeer.h \
    ipfs/ipfsid.h \
    ipfs/ipfsget.h \
    ipfs/ipfsls.h \
    directory.h \
    file.h \
    object.h \
    share.h \
    sharemodel.h \
    ipfs/ipfsrefs.h \
    ipfs/ipfsstats.h \
    objectiterator.h

RESOURCES += ui/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

DISTFILES += \
    README.md \
    LICENSE
