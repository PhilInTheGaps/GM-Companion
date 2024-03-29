cmake_minimum_required(VERSION 3.19)

set(SOURCES
    staticabstracttest.h
    staticabstracttest.cpp
    abstracttest.h
    abstracttest.cpp
    abstractmocknetworkmanager.h
    abstractmocknetworkmanager.cpp
    mocknetworkreply.h
    mocknetworkreply.cpp
    mockfs.h
    mockfs.cpp
)

qt_add_library(testhelper OBJECT ${SOURCES})

qt_add_resources(testhelper "test-resources"
    PREFIX "/"
    FILES
        resources/audioproject/project.audio
        resources/audiofiles/test.mp3
        resources/audiofiles/test.ogg
        resources/audiofiles/test.wav
        resources/audioplaylist/test.m3u
        resources/audioplaylist/test.pls
        resources/images/test.png
        resources/images/test.jpg
        resources/images/test.tif
        resources/images/test.gif
        resources/audioproject/project_v3.audio
        resources/units/units_v1.ini
        resources/shopproject/project.shop
        resources/shopproject/group.items)

qt_add_resources(testhelper "test-resources-addons"
    PREFIX "/"
    BASE resources
    FILES
        resources/addons/test/addon.json
        resources/addons/test/names/test.json
        resources/addons/test/audio/test.audio
        resources/addons/test/units/units.json
        resources/addon_repositories/testrepo.json
        resources/addons_remote/demo-addon-1.zip)

target_precompile_headers(testhelper REUSE_FROM common)
target_include_directories(testhelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(testhelper PUBLIC gm-companion-lib filesystem Qt6::Gui Qt6::Quick Qt6::Test gtest)
