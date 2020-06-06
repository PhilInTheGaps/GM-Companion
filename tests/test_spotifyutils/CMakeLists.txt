cmake_minimum_required(VERSION 3.1.0)

project(test_spotifyutils)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Test)

add_executable(test_spotifyutils testspotifyutils.h testspotifyutils.cpp)
target_link_libraries(test_spotifyutils gm-companion-lib Qt5::Test)
add_test(NAME test_spotifyutils COMMAND test_spotifyutils)