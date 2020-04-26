QT += testlib network

CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += ../../src

TEMPLATE = app

SOURCES += \
    ../../src/services/spotify/spotifyutils.cpp \
    testspotifyutils.cpp

HEADERS += \
    ../../src/services/spotify/spotifyutils.h \
    testspotifyutils.h
