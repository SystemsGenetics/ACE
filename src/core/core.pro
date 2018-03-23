
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = acecore
CONFIG += c++11
VERSION = 3.0.0

LIBS += -lmpi -lmpi_cxx -lOpenCL

TEMPLATE = lib

SOURCES += \
   opencldevicemodel.cpp \
   opencldevice.cpp \
   openclevent.cpp \
   openclprogram.cpp \
   openclkernel.cpp \
   utilities.cpp \
   exception.cpp \
   abstractanalyticfactory.cpp \
   opencl.cpp \
   datamanager.cpp \
   datareference.cpp \
   abstractanalytic.cpp \
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
    eabstractdatafactory.cpp

HEADERS += \
   opencldevicemodel.h \
   opencldevice.h \
   openclevent.h \
   openclbuffer.h \
   openclprogram.h \
   openclkernel.h \
   utilities.h \
   exception.h \
   abstractanalyticfactory.h \
   abstractanalytic.h \
   opencl.h \
   datamanager.h \
   datareference.h \
   AceCore.h \
   AceOpenCL.h \
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
    eabstractdatafactory.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
