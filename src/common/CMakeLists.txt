cmake_minimum_required(VERSION 3.14)

project(common LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  Qt5
  COMPONENTS Core Gui Network
  REQUIRED)
find_package(QtKeychain)

include(AddMarkdownLib)

# configure markdown lib
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h.in
               ${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h @ONLY)

set(SRC_UTILS
    utils/utils.h
    utils/utils.cpp
    utils/fileutils.h
    utils/fileutils.cpp
    utils/markdownutils.h
    utils/markdownutils.h.in
    utils/markdownutils.cpp
    utils/networkutils.h
    utils/networkutils.cpp
    utils/processinfo.h
    utils/processinfo.cpp
    utils/stringutils.h
    utils/stringutils.cpp)

set(SRC_MODELS
    models/treeitem.h
    models/treeitem.cpp)

set(SRC_SETTINGS
    settings/settingsmanager.h
    settings/settingsmanager.cpp
    settings/abstractsettingsmanager.h
    settings/abstractsettingsmanager.cpp
    settings/quicksettingsmanager.h
    settings/quicksettingsmanager.cpp)

set(SRC_UPDATES
    updates/updatemanager.h
    updates/updatemanager.cpp
    updates/version.h.in
    updates/version.h
    updates/version.cpp)

set(SRC_PROJECTS
    projects/abstractprojectupgrader.h
    projects/abstractprojectupgrader.cpp
    projects/abstractprojectupgraderjson.h
    projects/abstractprojectupgraderjson.cpp
    projects/abstractprojectupgraderini.h
    projects/abstractprojectupgraderini.cpp)

set(SOURCES
    ${SRC_UTILS}
    ${SRC_MODELS}
    ${SRC_SETTINGS}
    ${SRC_UPDATES}
    ${SRC_PROJECTS}
    logging.h
    logging.cpp
    abstracttool.h
    abstracttool.cpp
    platformdetails.h
    platformdetails.cpp
    exceptions/notimplementedexception.h)

add_library(common STATIC ${SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(common
    PRIVATE
    qtasyncfuture
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Network)
