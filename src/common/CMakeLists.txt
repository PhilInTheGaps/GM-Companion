cmake_minimum_required(VERSION 3.14)

project(common LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Gui Network REQUIRED)
find_package(QtKeychain)

include(AddMarkdownLib)

# configure markdown lib
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/markdownutils.h
  @ONLY)

set(HEADERS
    logging.h
    platformdetails.h
    abstracttool.h
    utils/utils.h 
    utils/fileutils.h 
    utils/markdownutils.h
    utils/markdownutils.h.in
    utils/networkutils.h 
    utils/processinfo.h 
    models/treeitem.h
    settings/settings.h
    settings/settingsmanager.h 
    updates/updatemanager.h
    updates/version.h.in
    updates/version.h
    exceptions/notimplementedexception.h
)

set(SOURCES
    logging.cpp 
    abstracttool.cpp
    platformdetails.cpp 
    utils/utils.cpp
    utils/fileutils.cpp 
    utils/markdownutils.cpp 
    utils/networkutils.cpp 
    utils/processinfo.cpp
    models/treeitem.cpp
    settings/settingsmanager.cpp
    updates/updatemanager.cpp
)

add_library(common STATIC ${HEADERS} ${SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(common
    PRIVATE
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Network
)
