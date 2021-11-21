cmake_minimum_required(VERSION 3.14)

project(filesystem LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Qml REQUIRED)

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

add_library(filesystem STATIC ${HEADERS} ${SOURCES})
target_compile_features(filesystem PUBLIC cxx_std_17)
target_include_directories(filesystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(filesystem
    PUBLIC
    qtasyncfuture
    propertyhelper
    common
    PRIVATE
    services
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
)
