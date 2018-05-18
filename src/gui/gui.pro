
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
    mainwindow.cpp \
    setupanalyticdialog.cpp \
    Application.cpp \
    analyticdialog.cpp \
    datawindow.cpp \
    metadatadialog.cpp \
    imageviewer.cpp

HEADERS += \
    opencldevicedialog.h \
    mainwindow.h \
    setupanalyticdialog.h \
    Application.h \
    analyticdialog.h \
    datawindow.h \
    metadatadialog.h \
    imageviewer.h

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
