
QT += core gui widgets
VERSION = 0.0.999

DESTDIR = $$PWD/../../build/libs/
TARGET = acegui
CONFIG += c++11

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

TEMPLATE = lib

SOURCES += \
    opencldevicedialog.cpp \
    setupanalyticdialog.cpp \
    analyticdialog.cpp \
    metadatadialog.cpp \
    imageviewer.cpp \
    eapplication.cpp \
    ace_mainwindow.cpp \
    ace_datawindow.cpp

HEADERS += \
    opencldevicedialog.h \
    setupanalyticdialog.h \
    analyticdialog.h \
    metadatadialog.h \
    imageviewer.h \
    eapplication.h \
    ace_mainwindow.h \
    ace_datawindow.h

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += \
    resources.qrc

QMAKE_CXX = mpic++

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd $${PWD}/../../build/libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/gui
includes.files = $${PWD}/*.h
INSTALLS += library includes
