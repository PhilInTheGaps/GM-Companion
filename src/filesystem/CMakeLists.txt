cmake_minimum_required(VERSION 3.19)

# set QML import path
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

qt_add_library(filesystem STATIC)

qt_add_qml_module(filesystem
    URI filesystem
    SOURCES
        file.h
        file.cpp
        fileaccess.h
        fileaccess.cpp
        fileaccesslocal.h
        fileaccesslocal.cpp
        fileaccessnextcloud.h
        fileaccessnextcloud.cpp
        fileaccessgoogledrive.h
        fileaccessgoogledrive.cpp
        fileaccessswitcher.h
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
        multigethelper.h
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
