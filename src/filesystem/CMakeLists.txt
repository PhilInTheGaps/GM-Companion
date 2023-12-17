cmake_minimum_required(VERSION 3.19)

set(SRC_COMMON
    file.h
    file.cpp
    fileaccess.h
    fileaccess.cpp
    fileaccessswitcher.h
    multigethelper.h
    options.h
    cache/filecache.h
    cache/filecache.cpp
    cache/filecacheentry.h
    cache/filecacheentry.cpp
    results/fileresult.h
    results/fileresult.cpp
    results/filelistresult.h
    results/filelistresult.cpp
    results/filecheckresult.h
    results/filecheckresult.cpp
    results/filemulticheckresult.h
    results/filedataresult.h
    results/filedataresult.cpp
)

set(SRC_LOCAL
    fileaccesslocal.h
    fileaccesslocal.cpp
)

set(SRC_NEXTCLOUD
    fileaccessnextcloud.h
    fileaccessnextcloud.cpp
)

set(SRC_GOOGLEDRIVE
    fileaccessgoogledrive.h
    fileaccessgoogledrive.cpp
    googledrive/endpoints.h
    googledrive/moveimpl.h
    googledrive/moveimpl.cpp
    googledrive/getdataimpl.h
    googledrive/getdataimpl.cpp
    googledrive/saveimpl.h
    googledrive/saveimpl.cpp
    googledrive/deleteimpl.h
    googledrive/deleteimpl.cpp
    googledrive/copyimpl.h
    googledrive/copyimpl.cpp
    googledrive/listimpl.h
    googledrive/listimpl.cpp
    googledrive/createfileimpl.h
    googledrive/createfileimpl.cpp
    googledrive/checkimpl.h
    googledrive/checkimpl.cpp
    googledrive/mimetype.h
)

qt_add_library(filesystem STATIC)

qt_add_qml_module(filesystem
    URI filesystem
    SOURCES
        ${SRC_COMMON}
        ${SRC_LOCAL}
        ${SRC_NEXTCLOUD}
        # ${SRC_GOOGLEDRIVE}
)

target_precompile_headers(filesystem REUSE_FROM common)

target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(filesystem
    PUBLIC
    common
    services
    propertyhelper
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
)
