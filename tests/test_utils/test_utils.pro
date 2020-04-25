QT += testlib

CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += ../../src
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../../src/utils/utils.cpp \
    testutils.cpp

HEADERS += \
    ../../src/utils/utils.h \
    testutils.h
