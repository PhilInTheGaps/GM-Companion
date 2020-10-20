cmake_minimum_required(VERSION 3.1.0)

project(test_utils)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Test)

# Utils
add_executable(test_utils testutils.cpp)
target_link_libraries(test_utils gm-companion-lib Qt5::Gui Qt5::Test)
add_test(NAME test_utils COMMAND test_utils)

# Spotify Utils
add_executable(test_spotifyutils testspotifyutils.cpp)
target_link_libraries(test_spotifyutils gm-companion-lib Qt5::Test)
add_test(NAME test_spotifyutils COMMAND test_spotifyutils)

# File Utils
add_executable(test_fileutils testfileutils.cpp)
target_link_libraries(test_fileutils gm-companion-lib Qt5::Test)
add_test(NAME test_fileutils COMMAND test_fileutils)

# Network Utils
add_executable(test_networkutils testnetworkutils.cpp)
target_link_libraries(test_networkutils gm-companion-lib Qt5::Test)
add_test(NAME test_networkutils COMMAND test_networkutils)

# Markdown Utils
add_executable(test_markdownutils testmarkdownutils.cpp)
target_link_libraries(test_markdownutils gm-companion-lib Qt5::Test)
add_test(NAME test_markdownutils COMMAND test_markdownutils)

# Process Info
add_executable(test_processinfo testprocessinfo.cpp)
target_link_libraries(test_processinfo gm-companion-lib Qt5::Test)
add_test(NAME test_processinfo COMMAND test_processinfo)
