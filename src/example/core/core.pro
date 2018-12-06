
include(../../ACE.pri)

DESTDIR = $$PWD/../../../build/libs/
TARGET = aceexcore
CONFIG += staticlib

INCLUDEPATH += $${PWD}/../../
DEPENDPATH += $${PWD}/../../

TEMPLATE = lib

SOURCES += \
    analyticfactory.cpp \
    datafactory.cpp \
    dataframe.cpp \
    dataframe_iterator.cpp \
    dataframe_model.cpp \
    mathtransform.cpp \
    importdataframe.cpp \
    importdataframe_input.cpp \
    exportdataframe.cpp \
    exportdataframe_input.cpp \
    mathtransform_input.cpp \
    mathtransform_serial.cpp \
    mathtransform_block.cpp \
    mathtransform_opencl.cpp \
    mathtransform_opencl_kernel.cpp \
    mathtransform_opencl_worker.cpp

HEADERS += \
    analyticfactory.h \
    datafactory.h \
    dataframe.h \
    dataframe_iterator.h \
    dataframe_model.h \
    mathtransform.h \
    importdataframe.h \
    importdataframe_input.h \
    exportdataframe.h \
    exportdataframe_input.h \
    mathtransform_input.h \
    mathtransform_serial.h \
    global.h \
    mathtransform_block.h \
    mathtransform_opencl.h \
    mathtransform_opencl_kernel.h \
    mathtransform_opencl_worker.h

QMAKE_CXXFLAGS += -Wno-ignored-attributes
