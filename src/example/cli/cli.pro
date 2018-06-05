
include(../../ACE.pri)
include(../example.pri)

TARGET = aceex

LIBS += -laceconsole

SOURCES += \
    ../main.cpp \

DEFINES += GUI=0

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${PWD}/../../../build/example/cli/$${TARGET}
INSTALLS += program
