QT += testlib network

CONFIG += qt console testcase
CONFIG -= app_bundle

include(../../lib/o2/src/src.pri)
INCLUDEPATH += ../../src

TEMPLATE = app

SOURCES += \
    ../../src/utils/processinfo.cpp\
    ../../src/settings/settingsmanager.cpp \
    ../../src/logging.cpp \
    ../../src/services/restserviceconnectorlocal.cpp \
    ../../src/services/spotify.cpp \
    ../../src/services/spotifyconnectorlocal.cpp \
    ../../src/services/spotifyconnectorserver.cpp \
    testspotify.cpp

HEADERS += \
    ../../src/utils/processinfo.h\
    ../../src/settings/settingsmanager.h \
    ../../src/logging.h \
    ../../src/services/restserviceconnector.h \
    ../../src/services/restserviceconnectorlocal.h \
    ../../src/services/services.h \
    ../../src/services/spotify.h \
    ../../src/services/spotifyconnectorlocal.h \
    ../../src/services/spotifyconnectorserver.h \
    testspotify.h

!win32 {
    LIBS += -L/usr/local/lib -lqt5keychain
    INCLUDEPATH += /usr/local/include
}

win32 {
    LIBS += -L$$PWD/../../lib/qt5keychain/lib -lqt5keychain.dll
    INCLUDEPATH += $$PWD/../../lib/qt5keychain/include
    DEPENDPATH  += $$PWD/../../lib/qt5keychain/include
}
