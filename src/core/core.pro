
QT += core
VERSION = 0.0.999

DESTDIR = $${PWD}/../../build/libs/
TARGET = acecore
CONFIG += c++11

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
    ace_analytic_singlerun.cpp \
    opencl_platform.cpp \
    opencl_device.cpp \
    opencl_common.cpp \
    opencl_context.cpp \
    opencl_program.cpp \
    opencl_event.cpp \
    opencl_commandqueue.cpp \
    opencl_kernel.cpp

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
    eabstractanalytic_block.h \
    opencl_platform.h \
    opencl.h \
    opencl_device.h \
    opencl_common.h \
    opencl_context.h \
    opencl_program.h \
    opencl_event.h \
    opencl_commandqueue.h \
    opencl_buffer.h \
    opencl_kernel.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd $${PWD}/../../build/libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/core
includes.files = $${PWD}/*.h
INSTALLS += library includes
