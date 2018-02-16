
QT += core

TARGET = example
CONFIG += c++11
GUI = 0

LIBS += -lmpi -lmpi_cxx -lOpenCL -L$$PWD/../../build/libs -lacecore

equals(GUI, 1) {
   QT += gui widgets
   LIBS += -lacegui
   DEFINES += ACE_GUI
}
else {
   LIBS += -laceconsole
}

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
