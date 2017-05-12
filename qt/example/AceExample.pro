#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T20:07:18
#
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION,5): error("Requires Qt 5")

QT += core gui widgets

TARGET = AceExample
TEMPLATE = app
VERSION = 9999

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++11

unix|win32: LIBS += -L$$PWD/../build-src/ -lace
unix|win32: LIBS += -lOpenCL

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-src/ace.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../build-src/libace.a


SOURCES += main.cpp \
    analyticfactory.cpp

HEADERS += \
    analyticfactory.h
