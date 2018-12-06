include(../ACE.pri)

TEMPLATE = app
TARGET = acelog

SOURCES += \
    main.cpp \
    socket.cpp \
    client.cpp

HEADERS += \
    socket.h \
    client.h

isEmpty(PREFIX) { PREFIX = /usr/local }
program.path = $${PREFIX}/bin
program.files = $${TARGET}
INSTALLS += program
