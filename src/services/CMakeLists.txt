cmake_minimum_required(VERSION 3.19)

set(SRC_SERVICES )

# Common
list(APPEND SRC_SERVICES
    service.h
    service.cpp
    servicestatus.h
    options.h
)

# REST
list(APPEND SRC_SERVICES
    rest/callbackserver.h
    rest/callbackserver.cpp
    rest/restserviceconnector.h
    rest/restserviceconnector.cpp
    rest/restserviceconnectorlocal.h
    rest/restserviceconnectorlocal.cpp
    rest/restreply.h
    rest/restreply.cpp
    rest/restrequest.h
)

# Spotify
list(APPEND SRC_SERVICES
    spotify/config.h
    spotify/spotify.h
    spotify/spotify.cpp
    spotify/spotifyutils.h
    spotify/spotifyutils.cpp
    spotify/spotifyconnectorlocal.h
    spotify/spotifyconnectorlocal.cpp
    spotify/spotifyconnectorserver.h
    spotify/spotifyconnectorserver.cpp
    spotify/spotifynetworkerror.h
    spotify/spotifynetworkerror.cpp
    spotify/clients/abstractspotifyclientcontroller.h
    spotify/clients/abstractspotifyclientcontroller.cpp
    spotify/clients/librespotcontroller.h
    spotify/clients/librespotcontroller.cpp
    spotify/data/basespotifyelement.h
    spotify/data/spotifyalbum.h
    spotify/data/spotifyalbum.cpp
    spotify/data/spotifyalbuminfo.h
    spotify/data/spotifyalbuminfo.cpp
    spotify/data/spotifyartist.h
    spotify/data/spotifyartist.cpp
    spotify/data/spotifydevice.h
    spotify/data/spotifydevice.cpp
    spotify/data/spotifyplaylist.h
    spotify/data/spotifyplaylist.cpp
    spotify/data/spotifytrack.h
    spotify/data/spotifytrack.cpp
    spotify/data/spotifytracklist.h
    spotify/data/spotifytracklist.cpp
    spotify/data/spotifycurrenttrack.h
    spotify/data/spotifycurrenttrack.cpp
    spotify/data/spotifyimage.h
    spotify/data/spotifyimage.cpp
    spotify/data/spotifyplaybackstate.h
    spotify/data/spotifyplaybackstate.cpp
    spotify/api/albumapi.h
    spotify/api/albumapi.cpp
    spotify/api/playerapi.h
    spotify/api/playerapi.cpp
    spotify/api/playlistsapi.h
    spotify/api/playlistsapi.cpp
    spotify/api/tracksapi.h
    spotify/api/tracksapi.cpp
)

# NextCloud
list(APPEND SRC_SERVICES
    nextcloud/nextcloud.h
    nextcloud/nextcloud.cpp
    nextcloud/nextcloudconnector.h
    nextcloud/nextcloudconnector.cpp
)

# Google Drive
if(WITH_GOOGLE_DRIVE)
    list(APPEND SRC_SERVICES
        google/googledrive.h
        google/googledrive.cpp
        google/googledriveconnectorlocal.h
        google/googledriveconnectorlocal.cpp
        google/googledrivereply.h
    )
endif()

# YouTube
if(WITH_YOUTUBE)
    list(APPEND SRC_SERVICES
        youtube/youtube.h
        youtube/youtube.cpp
        youtube/pipedconnector.h
        youtube/pipedconnector.cpp
        youtube/video.h
        youtube/playlist.h
        youtube/videoid.h
        youtube/videoid.cpp
        youtube/playlistid.h
        youtube/playlistid.cpp
    )
endif()

qt_add_library(services STATIC)

qt_add_qml_module(services
    URI services
    SOURCES ${SRC_SERVICES}
)

qt_add_resources(services "services-resources"
    PREFIX "/"
    BASE resources
    FILES
        resources/services/auth-success.html
        resources/services/nextcloud/getFileList.xml)

target_precompile_headers(services REUSE_FROM common)

set(SERVICES_PRIVATE_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/spotify
    ${CMAKE_CURRENT_SOURCE_DIR}/nextcloud
)

if (WITH_GOOGLE_DRIVE)
    list(APPEND SERVICES_PRIVATE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/google)
endif()

if (WITH_YOUTUBE)
    list(APPEND SERVICES_PRIVATE_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/youtube)
endif()

target_include_directories(services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${SERVICES_PRIVATE_INCLUDE})

target_link_libraries(services
    PUBLIC
    common
    propertyhelper
    o2
    GSL
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    PRIVATE
    http-status-codes
)
