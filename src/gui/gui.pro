
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
    eapplication.cpp \
    ace_mainwindow.cpp \
    ace_datawindow.cpp \
    ace_setupanalyticdialog.cpp \
    ace_analyticdialog.cpp \
    ace_analyticthread.cpp \
    ace_metadatadialog.cpp \
    ace_metadatadialog_imageviewdialog.cpp

HEADERS += \
    opencldevicedialog.h \
    eapplication.h \
    ace_mainwindow.h \
    ace_datawindow.h \
    ace_setupanalyticdialog.h \
    ace_analyticdialog.h \
    ace_analyticthread.h \
    ace.h \
    ace_metadatadialog.h \
    ace_metadatadialog_imageviewdialog.h

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
