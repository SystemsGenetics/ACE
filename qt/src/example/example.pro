
QT += core gui widgets

TARGET = example
CONFIG += c++11 link_prl

unix|win32: LIBS += -lOpenCL

LIBS += \
   $$PWD/../../build/core/libacecore.a \
   $$PWD/../../build/gui/libacegui.a

PRE_TARGETDEPS += \
   $$PWD/../../build/core/libacecore.a \
   $$PWD/../../build/gui/libacegui.a

INCLUDEPATH += ../

TEMPLATE = app

SOURCES += \
   main.cpp \
   analyticfactory.cpp \
   datafactory.cpp \
   integerarray.cpp \
   mathtransform.cpp

HEADERS += \
   analyticfactory.h \
   datafactory.h \
   integerarray.h \
   mathtransform.h

DEFINES += QT_DEPRECATED_WARNINGS
