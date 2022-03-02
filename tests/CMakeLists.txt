cmake_minimum_required(VERSION 3.1.0)

project(tests)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Enable extra tests?
set(ENABLE_EXTRA_TESTS OFF CACHE BOOL "Enable extra tests that can not run in a CI environment")
set(TEST_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(testhelper)

add_subdirectory(common)
add_subdirectory(filesystem)
add_subdirectory(services)
add_subdirectory(tools)
