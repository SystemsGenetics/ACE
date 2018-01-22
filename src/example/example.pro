
QT += core

TARGET = example
CONFIG += c++11

unix|win32: LIBS += -lOpenCL

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
    exportintegerarray.cpp

HEADERS += \
   analyticfactory.h \
   datafactory.h \
   integerarray.h \
   mathtransform.h \
    importintegerarray.h \
    exportintegerarray.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
