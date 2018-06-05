
QT += core gui widgets
VERSION = 0.0.999

TARGET = qaceex
CONFIG += c++11

LIBS += -lmpi -lmpi_cxx -lOpenCL -L$${PWD}/../../../build/libs -lacecore -lacegui -laceexcore

INCLUDEPATH += $${PWD}/../../ $${PWD}/../core/
DEPENDPATH += $${PWD}/../../ $${PWD}/../core/

TEMPLATE = app

SOURCES += \
    ../main.cpp \

RESOURCES += \
    ../resources.qrc

DEFINES += QT_DEPRECATED_WARNINGS GUI=1

QMAKE_CXX = mpic++
QMAKE_CXXFLAGS += -Wno-ignored-attributes

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${PWD}/../../../build/example/gui/$${TARGET}
INSTALLS += program
