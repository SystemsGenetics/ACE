
QT += core gui widgets

DESTDIR = $$PWD/../../build/libs/
TARGET = acegui
CONFIG += c++11
VERSION = 0.0.999

LIBS += -lOpenCL -L$$PWD/../../build/libs -lacecore

INCLUDEPATH += $$PWD/../core/
DEPENDPATH += $$PWD/../core/

TEMPLATE = lib

SOURCES += \
   opencldevicedialog.cpp \
   mainwindow.cpp \
   setupanalyticdialog.cpp \
   Application.cpp

HEADERS += \
   opencldevicedialog.h \
   mainwindow.h \
   setupanalyticdialog.h \
   Application.h

DEFINES += QT_DEPRECATED_WARNINGS
