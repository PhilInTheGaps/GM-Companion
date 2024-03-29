cmake_minimum_required(VERSION 3.19)

# set QML import path
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND QML_IMPORT_PATH ${CMAKE_CURRENT_BINARY_DIR}/../)
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

qt_add_qml_module(gm-companion-ui-filedialog
    STATIC
    URI FileDialog
    SOURCES
        filedialogbackend.h
        filedialogbackend.cpp
        fileobject.h
)

target_precompile_headers(gm-companion-ui-filedialog REUSE_FROM common)

target_include_directories(gm-companion-ui-filedialog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gm-companion-ui-filedialog
    PUBLIC
    common
    services
    filesystem
    propertyhelper
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
)
