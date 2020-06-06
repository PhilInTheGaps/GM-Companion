cmake_minimum_required(VERSION 3.1.0)

project(test_utils)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Test)

add_executable(test_utils testutils.h testutils.cpp)
target_link_libraries(test_utils gm-companion-lib Qt5::Gui Qt5::Test)
add_test(NAME test_utils COMMAND test_utils)