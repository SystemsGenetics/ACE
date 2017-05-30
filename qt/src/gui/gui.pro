
QT += core gui widgets

TARGET = acegui
CONFIG += c++11 staticlib create_prl

unix|win32: LIBS += -lOpenCL

LIBS += $$PWD/../../build/core/libacecore.a

PRE_TARGETDEPS += $$PWD/../../build/core/libacecore.a

INCLUDEPATH += ../core/

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
