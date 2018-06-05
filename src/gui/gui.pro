
include(../ACE.pri)

QT += core gui widgets

DESTDIR = $$PWD/../../build/libs/
TARGET = acegui
CONFIG += c++11

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

TEMPLATE = lib

SOURCES += \
    eapplication.cpp \
    ace_mainwindow.cpp \
    ace_datawindow.cpp \
    ace_setupanalyticdialog.cpp \
    ace_analyticdialog.cpp \
    ace_analyticthread.cpp \
    ace_metadatadialog.cpp \
    ace_metadatadialog_imageviewdialog.cpp \
    ace_settingsdialog.cpp

HEADERS += \
    eapplication.h \
    ace_mainwindow.h \
    ace_datawindow.h \
    ace_setupanalyticdialog.h \
    ace_analyticdialog.h \
    ace_analyticthread.h \
    ace.h \
    ace_metadatadialog.h \
    ace_metadatadialog_imageviewdialog.h \
    ace_settingsdialog.h

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
