
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = aceconsole
CONFIG += c++11
VERSION = 0.0.999

LIBS += -lOpenCL -L$$PWD/../../build/libs -lacecore

INCLUDEPATH += $$PWD/../core/
DEPENDPATH += $$PWD/../core/

TEMPLATE = lib

SOURCES += \
    Application.cpp

HEADERS += \
    Application.h

DEFINES += QT_DEPRECATED_WARNINGS

