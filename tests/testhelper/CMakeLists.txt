cmake_minimum_required(VERSION 3.19)

# resources
set(RESOURCES
  ../tests.qrc
)

set(SOURCES
    abstracttest.h
    abstracttest.cpp
    abstractmocknetworkmanager.h
    abstractmocknetworkmanager.cpp
    mocknetworkreply.h
    mocknetworkreply.cpp
    mockfs.h
    mockfs.cpp
)

qt_add_library(testhelper STATIC ${SOURCES} ${RESOURCES})
target_include_directories(testhelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(testhelper PUBLIC gm-companion-lib filesystem Qt6::Gui Qt6::Quick Qt6::Test)
