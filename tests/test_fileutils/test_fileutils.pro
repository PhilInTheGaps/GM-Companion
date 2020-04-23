QT += testlib

CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += ../../src
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../../src/utils/fileutils.cpp \
    testfileutils.cpp

HEADERS += \
    ../../src/utils/fileutils.h \
    testfileutils.h
