
include(../../ACE.pri)
include(../example.pri)

QT += gui widgets

TARGET = qaceex

LIBS += -lacegui

SOURCES += \
    ../main.cpp \

DEFINES += GUI=1

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${TARGET}
INSTALLS += program
