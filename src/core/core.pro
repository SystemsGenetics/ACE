
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
   abstractdata.cpp \
   datastream.cpp \
   dataobject.cpp \
   abstractdatafactory.cpp \
   opencl.cpp \
   datamanager.cpp \
   datareference.cpp \
   abstractanalytic.cpp \
   metadata.cpp \
   emetadata.cpp \
   ace_qmpi.cpp \
    common.cpp \
    emetaarray.cpp \
    emetaobject.cpp \
    old_metadatamodel.cpp \
    ace_metadatamodel.cpp \
    ace_metadatamodel_node.cpp

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
   abstractdata.h \
   datastream.h \
   dataobject.h \
   abstractdatafactory.h \
   opencl.h \
   datamanager.h \
   datareference.h \
   AceCore.h \
   AceOpenCL.h \
   metadata.h \
   emetadata.h \
   ace_qmpi.h \
    common.h \
    emetaarray.h \
    emetaobject.h \
    old_metadatamodel.h \
    ace_metadatamodel.h \
    ace_metadatamodel_node.h

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXX = mpic++
