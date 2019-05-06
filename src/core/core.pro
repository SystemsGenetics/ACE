
include(../ACE.pri)

DESTDIR = $${PWD}/../../build/libs/
TARGET = acecore

TEMPLATE = lib

SOURCES += \
    global.cpp \
    emetadata.cpp \
    ace_qmpi.cpp \
    emetaarray.cpp \
    emetaobject.cpp \
    ace_metadatamodel.cpp \
    ace_metadatamodelnode.cpp \
    ace_dataobject.cpp \
    edatastream.cpp \
    eabstractdata.cpp \
    eabstractdatafactory.cpp \
    ace_datamanager.cpp \
    eexception.cpp \
    eabstractanalytic.cpp \
    eabstractanalyticinput.cpp \
    eabstractanalyticblock.cpp \
    eabstractanalyticserial.cpp \
    eabstractanalyticopencl.cpp \
    eabstractanalyticopenclworker.cpp \
    eabstractanalyticcuda.cpp \
    eabstractanalyticcudaworker.cpp \
    eabstractanalyticfactory.cpp \
    opencl_platform.cpp \
    opencl_device.cpp \
    opencl.cpp \
    opencl_context.cpp \
    opencl_program.cpp \
    opencl_event.cpp \
    opencl_commandqueue.cpp \
    opencl_kernel.cpp \
    ace_analytic_simplerun.cpp \
    ace_analytic_serialrun.cpp \
    ace_analytic_openclrun.cpp \
    ace_analytic_openclrunthread.cpp \
    ace_analytic_cudarun.cpp \
    ace_analytic_cudarun_thread.cpp \
    ace_analytic_single.cpp \
    ace_settings.cpp \
    ace_analytic_mpimaster.cpp \
    ace_analytic_mpislave.cpp \
    ace_analytic_chunk.cpp \
    ace_analytic_merge.cpp \
    ace_analytic_abstractrun.cpp \
    ace_analytic_abstractinput.cpp \
    ace_analytic_abstractmanager.cpp \
    ace_analytic_abstractmpi.cpp \
    ace_logserver.cpp \
    elog.cpp \
    edebug.cpp \
    cuda_common.cpp \
    cuda_context.cpp \
    cuda_device.cpp \
    cuda_event.cpp \
    cuda_kernel.cpp \
    cuda_program.cpp \
    cuda_stream.cpp

HEADERS += \
    opencl.h \
    emetadata.h \
    ace_qmpi.h \
    emetaarray.h \
    emetaobject.h \
    ace_metadatamodel.h \
    ace_metadatamodelnode.h \
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
    eabstractanalyticinput.h \
    eabstractanalyticblock.h \
    eabstractanalyticserial.h \
    eabstractanalyticopencl.h \
    eabstractanalyticopenclworker.h \
    eabstractanalyticcuda.h \
    eabstractanalyticcudaworker.h \
    eabstractanalyticfactory.h \
    ace_analytic.h \
    core.h \
    opencl_platform.h \
    opencl.h \
    opencl_device.h \
    opencl_context.h \
    opencl_program.h \
    opencl_event.h \
    opencl_commandqueue.h \
    opencl_buffer.h \
    opencl_kernel.h \
    ace_analytic_simplerun.h \
    ace_analytic_serialrun.h \
    ace_analytic_openclrun.h \
    ace_analytic_openclrunthread.h \
    ace_analytic_cudarun.h \
    ace_analytic_cudarun_thread.h \
    ace_analytic_single.h \
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
    edebug.h \
    cuda_buffer.h \
    cuda_common.h \
    cuda_context.h \
    cuda_device.h \
    cuda_event.h \
    cuda_kernel.h \
    cuda_program.h \
    cuda_stream.h \
    cudaxx.h

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd ../libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/core
includes.files = $${PWD}/*.h
INSTALLS += library includes
