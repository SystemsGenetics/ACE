
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = aceconsole
CONFIG += c++11
VERSION = 3.0.0

INCLUDEPATH += $$PWD/../core/
DEPENDPATH += $$PWD/../core/

TEMPLATE = lib

SOURCES += \
    Application.cpp

HEADERS += \
    Application.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
