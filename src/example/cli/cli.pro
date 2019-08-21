
include(../../ACE.pri)
include(../example.pri)

TARGET = aceex

LIBS += -lacecli

SOURCES += \
    ../main.cpp \

DEFINES += GUI=0

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${TARGET}
INSTALLS += program
