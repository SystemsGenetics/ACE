
QT += core

DESTDIR = $$PWD/../../build/libs/
TARGET = acecore
CONFIG += c++11
VERSION = 0.0.999

unix|win32: LIBS += -lOpenCL

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
    metadatavalue.cpp \
    metadataarray.cpp \
    metadataobject.cpp \
    metadatamodel.cpp

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
    metadatavalue.h \
    metadataarray.h \
    metadataobject.h \
    metadatamodel.h

DEFINES += QT_DEPRECATED_WARNINGS
