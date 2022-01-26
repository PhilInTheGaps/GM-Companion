cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)

# resources
set(RESOURCES
  ../tests.qrc
)

set(HEADERS
    abstracttest.h
)

set(SOURCES
    abstracttest.cpp
)

add_library(testhelper STATIC ${HEADERS} ${SOURCES} ${RESOURCES})

target_include_directories(testhelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(testhelper filesystem Qt5::Gui Qt5::Quick Qt5::Test)
