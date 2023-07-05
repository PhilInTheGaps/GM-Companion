cmake_minimum_required(VERSION 3.14)

find_package(Qt6 COMPONENTS Core Gui Network REQUIRED)
find_package(QtKeychain REQUIRED)

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
    models/treeitem.cpp
    models/customobjectlistmodel.h
    models/customobjectlistmodel.cpp
    models/baseprojectitem.h
    models/baseprojectitem.cpp)

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
    abstracttool.h
    abstracttool.cpp
    exceptions/notimplementedexception.h)

qt_add_library(common STATIC ${SOURCES})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(common PRIVATE
    qtasyncfuture
    propertyhelper
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt6::Core
    Qt6::Gui
    Qt6::Network)
