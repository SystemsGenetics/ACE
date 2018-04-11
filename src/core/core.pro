
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = acecore
CONFIG += c++11
VERSION = 3.0.0

LIBS += -lmpi -lmpi_cxx -lOpenCL

TEMPLATE = lib

SOURCES += \
   emetadata.cpp \
   ace_qmpi.cpp \
    common.cpp \
    emetaarray.cpp \
    emetaobject.cpp \
    ace_metadatamodel.cpp \
    ace_metadatamodel_node.cpp \
    ace_dataobject.cpp \
    edatastream.cpp \
    eabstractdata.cpp \
    eabstractdatafactory.cpp \
    ace_datamanager.cpp \
    eexception.cpp \
    ace_analytic_manager.cpp \
    eabstractanalytic.cpp \
    eabstractanalytic_input.cpp \
    eabstractanalytic_serial.cpp \
    eabstractanalyticfactory.cpp \
    ace_analytic_singlerun.cpp

HEADERS += \
   opencl.h \
   emetadata.h \
   ace_qmpi.h \
    common.h \
    emetaarray.h \
    emetaobject.h \
    ace_metadatamodel.h \
    ace_metadatamodel_node.h \
    ace_dataobject.h \
    edatastream.h \
    edatastream_ext.h \
    global.h \
    ace.h \
    eabstractdata.h \
    eabstractdatafactory.h \
    ace_datamanager.h \
    eexception.h \
    ace_analytic_manager.h \
    eabstractanalytic.h \
    eabstractanalytic_input.h \
    eabstractanalytic_serial.h \
    eabstractanalyticfactory.h \
    ace_analytic_singlerun.h \
    ace_analytic.h \
    core.h \
    eabstractanalytic_block.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
