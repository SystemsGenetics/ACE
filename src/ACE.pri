
MAJOR_VERSION = 0
MINOR_VERSION = 0
REVISION = 999

VERSION = $${MAJOR_VERSION}.$${MINOR_VERSION}.$${REVISION}

DEFINES += \
    QT_DEPRECATED_WARNINGS \
    MAJOR_VERSION=$${MAJOR_VERSION} \
    MINOR_VERSION=$${MINOR_VERSION} \
    REVISION=$${REVISION}

QT += core

QMAKE_CXX = mpic++
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11