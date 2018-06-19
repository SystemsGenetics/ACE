
isEmpty(MPICXX) { MPICXX = "yes" }

LIBS += -lmpi
equals(MPICXX,"yes") { LIBS += -lmpi_cxx }
LIBS += -lOpenCL -L$${PWD}/../../build/libs -lacecore -laceexcore

INCLUDEPATH += $${PWD}/../ $${PWD}/core/
DEPENDPATH += $${PWD}/../ $${PWD}/core/

TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-ignored-attributes

RESOURCES += \
    ../resources.qrc
