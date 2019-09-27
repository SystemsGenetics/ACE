
isEmpty(MPICXX) { MPICXX = "yes" }

LIBS += -L$${OUT_PWD}/../libs -L$${OUT_PWD}/../../libs -laceexcore -lacecore -lcuda -lnvrtc -lOpenCL -lmpi
equals(MPICXX,"yes") { LIBS += -lmpi_cxx }

INCLUDEPATH += $${PWD}/../ $${PWD}/core/
DEPENDPATH += $${PWD}/../ $${PWD}/core/
PRE_TARGETDEPS += $${OUT_PWD}/../libs/libaceexcore.a

TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-ignored-attributes

RESOURCES += \
    ../resources.qrc
