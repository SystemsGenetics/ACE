
QT += core
VERSION = 0.0.999

DESTDIR = $$PWD/../../../build/libs/
TARGET = aceexcore
CONFIG += c++11 staticlib

LIBS += -lmpi -lmpi_cxx -lOpenCL -L$${PWD}/../../../build/libs -lacecore

INCLUDEPATH += $${PWD}/../../
DEPENDPATH += $${PWD}/../../

TEMPLATE = lib

SOURCES += \
    analyticfactory.cpp \
    datafactory.cpp \
    integerarray.cpp \
    mathtransform.cpp \
    importintegerarray.cpp \
    exportintegerarray.cpp \
    mathtransform_input.cpp \
    mathtransform_serial.cpp \
    mathtransform_block.cpp \
    importintegerarray_input.cpp \
    exportintegerarray_input.cpp \
    mathtransform_opencl.cpp \
    mathtransform_opencl_kernel.cpp \
    mathtransform_opencl_worker.cpp

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
    mathtransform_block.h \
    importintegerarray_input.h \
    exportintegerarray_input.h \
    mathtransform_opencl.h \
    mathtransform_opencl_kernel.h \
    mathtransform_opencl_worker.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
QMAKE_CXXFLAGS += -Wno-ignored-attributes
