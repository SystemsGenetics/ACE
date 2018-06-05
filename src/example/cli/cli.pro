
include(../../ACE.pri)

QT += core

TARGET = aceex
CONFIG += c++11

LIBS += -lmpi -lmpi_cxx -lOpenCL -L$${PWD}/../../../build/libs -lacecore -laceconsole -laceexcore

INCLUDEPATH += $${PWD}/../../ $${PWD}/../core/
DEPENDPATH += $${PWD}/../../ $${PWD}/../core/

TEMPLATE = app

SOURCES += \
    ../main.cpp \

RESOURCES += \
    ../resources.qrc

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
QMAKE_CXXFLAGS += -Wno-ignored-attributes

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${PWD}/../../../build/example/cli/$${TARGET}
INSTALLS += program
