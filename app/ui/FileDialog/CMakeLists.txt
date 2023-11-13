cmake_minimum_required(VERSION 3.19)

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
