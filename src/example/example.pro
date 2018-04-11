
QT += core

TARGET = example
CONFIG += c++11

LIBS += -lmpi -lmpi_cxx -lOpenCL -L$$PWD/../../build/libs -lacecore -laceconsole

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

TEMPLATE = app

SOURCES += \
   main.cpp \
   analyticfactory.cpp \
   datafactory.cpp \
   integerarray.cpp \
   mathtransform.cpp \
   importintegerarray.cpp \
   exportintegerarray.cpp \
    mathtransform_input.cpp \
    mathtransform_serial.cpp \
    mathtransform_block.cpp

HEADERS += \
   analyticfactory.h \
   datafactory.h \
   integerarray.h \
   mathtransform.h \
   importintegerarray.h \
   exportintegerarray.h \
    mathtransform_input.h \
    mathtransform_serial.h \
    global.h \
    mathtransform_block.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
