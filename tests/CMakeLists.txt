cmake_minimum_required(VERSION 3.19)

project(tests)

set(TEST_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Network Test)

add_subdirectory(testhelper)

add_subdirectory(common)
add_subdirectory(filesystem)
add_subdirectory(services)
add_subdirectory(tools)
add_subdirectory(addons)
