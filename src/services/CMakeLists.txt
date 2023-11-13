cmake_minimum_required(VERSION 3.19)

set(SRC_COMMON
    service.h
    service.cpp
    servicestatus.h
)

set(SRC_REST
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

set(SRC_SPOTIFY
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

set(SRC_NEXTCLOUD
    nextcloud/nextcloud.h
    nextcloud/nextcloud.cpp
)

set(SRC_GOOGLEDRIVE
    google/googledrive.h
    google/googledrive.cpp
    google/googledriveconnectorlocal.h
    google/googledriveconnectorlocal.cpp
    google/googledrivereply.h
)

qt_add_library(services STATIC)

qt_add_qml_module(services
    URI services
    SOURCES
        ${SRC_COMMON}
        ${SRC_REST}
        ${SRC_SPOTIFY}
        ${SRC_NEXTCLOUD}
        # GoogleDrive support is disabled, see comment in googledrive.h
        # ${SRC_GOOGLEDRIVE}
)

qt_add_resources(services "services-resources"
    PREFIX "/"
    BASE resources
    FILES
        resources/services/auth-success.html
        resources/services/nextcloud/getFileList.xml)

target_precompile_headers(services REUSE_FROM common)

target_include_directories(services
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/spotify
        ${CMAKE_CURRENT_SOURCE_DIR}/nextcloud)
        # ${CMAKE_CURRENT_SOURCE_DIR}/google

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
