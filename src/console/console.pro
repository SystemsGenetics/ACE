
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = aceconsole
CONFIG += c++11
VERSION = 3.0.0

INCLUDEPATH += $$PWD/../core/
DEPENDPATH += $$PWD/../core/

TEMPLATE = lib

SOURCES += \
    ace_options.cpp \
    ace_command.cpp \
    ace_run.cpp \
    eapplication.cpp

HEADERS += \
    ace_options.h \
    ace_command.h \
    ace_run.h \
    ace.h \
    eapplication.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
