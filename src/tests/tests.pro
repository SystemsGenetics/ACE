
QT += core testlib

TARGET = tests
CONFIG += c++11

LIBS += -lOpenCL -L$$PWD/../../build/libs -lacecore

INCLUDEPATH += $$PWD/../core/
DEPENDPATH += $$PWD/../core/

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    testexception.h \
    testdatastream.h \
    testdataobject.h \
    dummydata.h \
    dummydatafactory.h \
    testdatamanager.h \
    testmetadata.h \
    testopencldevice.h \
    testopenclprogram.h \
    testopenclkernel.h \
    testopenclbuffer.h \
    testopenclevent.h

SOURCES += \
    testexception.cpp \
    main.cpp \
    testdatastream.cpp \
    testdataobject.cpp \
    dummydatafactory.cpp \
    testdatamanager.cpp \
    testmetadata.cpp \
    testopencldevice.cpp \
    testopenclprogram.cpp \
    testopenclkernel.cpp \
    testopenclbuffer.cpp \
    testopenclevent.cpp

RESOURCES += \
    resources.qrc
