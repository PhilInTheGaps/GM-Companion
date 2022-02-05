cmake_minimum_required(VERSION 3.14)

project(services LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)

include(AddO2)

set(HEADERS
    service.h
    services.h
    servicestatus.h
    rest/restserviceconnector.h
    rest/restserviceconnectorlocal.h
    rest/restnetworkreply.h
    rest/requestcontainer.h
    spotify/config.h
    spotify/spotify.h
    spotify/spotifyutils.h
    spotify/spotifyconnectorlocal.h
    spotify/spotifyconnectorserver.h
    spotify/spotifynetworkerror.h
    spotify/clients/abstractspotifyclientcontroller.h
    spotify/clients/librespotcontroller.h
    spotify/data/spotifydevice.h
    nextcloud/nextcloud.h
    google/googledrive.h
    google/googledriveconnectorlocal.h
    google/googledrivereply.h
    discord/discord.h
)

set(SOURCES
    service.cpp
    rest/restserviceconnectorlocal.cpp
    rest/restnetworkreply.cpp
    spotify/spotify.cpp
    spotify/spotifyutils.cpp
    spotify/spotifyconnectorlocal.cpp
    spotify/spotifyconnectorserver.cpp
    spotify/spotifynetworkerror.cpp
    spotify/clients/abstractspotifyclientcontroller.cpp
    spotify/clients/librespotcontroller.cpp
    spotify/data/spotifydevice.cpp
    nextcloud/nextcloud.cpp
    google/googledrive.cpp
    google/googledriveconnectorlocal.cpp
    discord/discord.cpp
)

add_library(services STATIC ${HEADERS} ${SOURCES})

target_include_directories(services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(services
    common
    o2
    qtasyncfuture
    http-status-codes
    Qt5::Core
    Qt5::Gui
    Qt5::Network
)
