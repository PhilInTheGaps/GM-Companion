QT += testlib

CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += ../../src
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../../src/utils/networkutils.cpp \
    testnetworkutils.cpp

HEADERS += \
    ../../src/utils/networkutils.h \
    testnetworkutils.h
