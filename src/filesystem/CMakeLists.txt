cmake_minimum_required(VERSION 3.19)

set(SRC_FILESYSTEM )

# Common
list(APPEND SRC_FILESYSTEM
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

# Local
list(APPEND SRC_FILESYSTEM
    fileaccesslocal.h
    fileaccesslocal.cpp
)

# NextCloud
list(APPEND SRC_FILESYSTEM
    fileaccessnextcloud.h
    fileaccessnextcloud.cpp
)

# Google Drive
if(WITH_GOOGLE_DRIVE)
    list(APPEND SRC_FILESYSTEM
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
endif()

qt_add_library(filesystem STATIC)

qt_add_qml_module(filesystem
    URI filesystem
    SOURCES ${SRC_FILESYSTEM}
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
