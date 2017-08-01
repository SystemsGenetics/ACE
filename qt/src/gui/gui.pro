
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
