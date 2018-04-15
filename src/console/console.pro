
QT += core
VERSION = 0.0.999

DESTDIR = $$PWD/../../build/libs/
TARGET = aceconsole
CONFIG += c++11

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

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd $${PWD}/../../build/libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/console
includes.files = $${PWD}/*.h
INSTALLS += library includes
