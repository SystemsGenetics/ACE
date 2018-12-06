
isEmpty(MPICXX) { MPICXX = "yes" }

LIBS += -L$${PWD}/../../build/libs -laceexcore -lacecore -L$${CUDADIR}/lib64 -lcuda -lnvrtc -lOpenCL -lmpi
equals(MPICXX,"yes") { LIBS += -lmpi_cxx }

INCLUDEPATH += $${PWD}/../ $${PWD}/core/
DEPENDPATH += $${PWD}/../ $${PWD}/core/
PRE_TARGETDEPS += ../../libs/libaceexcore.a

TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-ignored-attributes

RESOURCES += \
    ../resources.qrc
