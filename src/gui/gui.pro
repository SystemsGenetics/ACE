
include(../ACE.pri)

QT += gui widgets

DESTDIR = $$PWD/../../build/libs/
TARGET = acegui

TEMPLATE = lib

SOURCES += \
    eapplication.cpp \
    ace_mainwindow.cpp \
    ace_datawindow.cpp \
    ace_setupanalyticdialog.cpp \
    ace_analyticthread.cpp \
    ace_analyticdialog.cpp \
    ace_metadatadialog.cpp \
    ace_metadatadialog_imageviewdialog.cpp \
    ace_settingsdialog.cpp

HEADERS += \
    eapplication.h \
    ace_mainwindow.h \
    ace_datawindow.h \
    ace_setupanalyticdialog.h \
    ace_analyticthread.h \
    ace_analyticdialog.h \
    ace.h \
    ace_metadatadialog.h \
    ace_metadatadialog_imageviewdialog.h \
    ace_settingsdialog.h

RESOURCES += \
    resources.qrc

isEmpty(PREFIX) { PREFIX = /usr/local }
library.path = $${PREFIX}/lib
library.extra = cp -fd ../libs/lib$${TARGET}.so* $${PREFIX}/lib/
includes.path = $${PREFIX}/include/ace/gui
includes.files = $${PWD}/*.h
INSTALLS += library includes
