QT       += core network qml quick sql

CONFIG += c++11
CONFIG += debug_and_release

TARGET = Arbore-qt
TEMPLATE = app

CXX = $$(CXX)
if(!isEmpty(CXX)) {
    QMAKE_CXX = $$CXX
}

CC = $$(CC)
if(!isEmpty(CC)) {
    QMAKE_CC = $$CC
}

QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_CFLAGS += $$(CFLAGS)
QMAKE_LFLAGS += $$(LDFLAGS)

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
    objectiterator.cpp \
    persist/sharerepository.cpp \
    persist/persist.cpp \
    objectcache.cpp

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
    objectiterator.h \
    persist/sharerepository.h \
    persist/persist.h \
    objectcache.h

RESOURCES += ui/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

DISTFILES += \
    README.md \
    LICENSE
