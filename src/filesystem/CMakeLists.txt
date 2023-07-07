cmake_minimum_required(VERSION 3.14)

find_package(Qt6 COMPONENTS Core Quick Qml REQUIRED)

set(HEADERS
    file.h
    fileaccess.h
    fileaccesslocal.h
    fileaccessnextcloud.h
    fileaccessgoogledrive.h
    fileaccessswitcher.h
    multigethelper.h
    results/fileresult.h
    results/filedataresult.h
    results/filelistresult.h
    results/filecheckresult.h
    results/filemulticheckresult.h
    cache/filecache.h
    cache/filecacheentry.h
    filedialog/filedialog.h
    filedialog/fileobject.h
)

set(SOURCES
    file.cpp
    fileaccess.cpp
    fileaccesslocal.cpp
    fileaccessnextcloud.cpp
    fileaccessgoogledrive.cpp
    cache/filecache.cpp
    cache/filecacheentry.cpp
    filedialog/filedialog.cpp
    results/fileresult.cpp
    results/filelistresult.cpp
    results/filecheckresult.cpp
    results/filedataresult.cpp
)

qt_add_library(filesystem STATIC ${HEADERS} ${SOURCES})
target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(filesystem
    PUBLIC
    propertyhelper
    common
    PRIVATE
    services
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
)
