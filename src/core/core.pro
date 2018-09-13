
include(../ACE.pri)

DESTDIR = $${PWD}/../../build/libs/
TARGET = acecore

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
    eabstractanalytic.cpp \
    eabstractanalytic_input.cpp \
    eabstractanalytic_serial.cpp \
    eabstractanalyticfactory.cpp \
    opencl_platform.cpp \
    opencl_device.cpp \
    opencl_common.cpp \
    opencl_context.cpp \
    opencl_program.cpp \
    opencl_event.cpp \
    opencl_commandqueue.cpp \
    opencl_kernel.cpp \
    eabstractanalytic_opencl.cpp \
    eabstractanalytic_block.cpp \
    opencl_kernel_locker.cpp \
    ace_analytic_simplerun.cpp \
    ace_analytic_serialrun.cpp \
    ace_analytic_openclrun.cpp \
    ace_analytic_openclrun_thread.cpp \
    ace_analytic_single.cpp \
    ace_settings.cpp \
    ace_analytic_mpimaster.cpp \
    ace_analytic_mpislave.cpp \
    ace_analytic_chunk.cpp \
    ace_analytic_merge.cpp \
    ace_analytic_abstractrun.cpp \
    ace_analytic_abstractinput.cpp \
    ace_analytic_abstractmanager.cpp \
    eabstractanalytic_opencl_worker.cpp \
    ace_analytic_abstractmpi.cpp \
    ace_logserver.cpp \
    elog.cpp \
    edebug.cpp

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
    eabstractanalytic.h \
    eabstractanalytic_input.h \
    eabstractanalytic_serial.h \
    eabstractanalyticfactory.h \
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
    opencl_kernel.h \
    eabstractanalytic_opencl.h \
    opencl_kernel_locker.h \
    ace_analytic_simplerun.h \
    ace_analytic_serialrun.h \
    ace_analytic_openclrun.h \
    ace_analytic_openclrun_thread.h \
    ace_analytic_single.h \
    eabstractanalytic_opencl_worker.h \
    openclxx.h \
    ace_settings.h \
    ace_analytic_mpimaster.h \
    ace_analytic_mpislave.h \
    ace_analytic_chunk.h \
    ace_analytic_merge.h \
    ace_analytic_abstractrun.h \
    ace_analytic_abstractinput.h \
    ace_analytic_abstractmanager.h \
    ace_analytic_abstractmpi.h \
    ace_logserver.h \
    elog.h \
    edebug.h

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd $${PWD}/../../build/libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/core
includes.files = $${PWD}/*.h
INSTALLS += library includes
