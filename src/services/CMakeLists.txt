cmake_minimum_required(VERSION 3.14)

project(services LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)

include(AddO2)

set(HEADERS
    service.h
    services.h
    servicestatus.h
    rest/restserviceconnector.h
    rest/restserviceconnectorlocal.h
    spotify/spotify.h
    spotify/spotifyutils.h
    spotify/spotifyconnectorlocal.h
    spotify/spotifyconnectorserver.h
    nextcloud/nextcloud.h
    google/googledrive.h
    google/googledriveconnectorlocal.h
    discord/discord.h
)

set(SOURCES
    service.cpp
    rest/restserviceconnectorlocal.cpp
    spotify/spotify.cpp
    spotify/spotifyutils.cpp
    spotify/spotifyconnectorlocal.cpp
    spotify/spotifyconnectorserver.cpp
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
    Qt5::Core
    Qt5::Gui
    Qt5::Network
)
