
include(../ACE.pri)

DESTDIR = $$PWD/../../build/libs/
TARGET = aceconsole

TEMPLATE = lib

SOURCES += \
    ace_options.cpp \
    ace_command.cpp \
    ace_run.cpp \
    eapplication.cpp \
    ace_settingsrun.cpp

HEADERS += \
    ace_options.h \
    ace_command.h \
    ace_run.h \
    ace.h \
    eapplication.h \
    ace_settingsrun.h

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd $${PWD}/../../build/libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/console
includes.files = $${PWD}/*.h
INSTALLS += library includes
