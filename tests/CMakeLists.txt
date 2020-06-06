cmake_minimum_required(VERSION 3.1.0)

project(tests)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(test_fileutils)
add_subdirectory(test_networkutils)
add_subdirectory(test_spotifyutils)
add_subdirectory(test_utils)
