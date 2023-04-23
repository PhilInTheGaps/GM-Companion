cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)

# Project Upgrader
add_executable(test_converterprojectupgrader testconverterprojectupgrader.cpp)
target_link_libraries(test_converterprojectupgrader gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_converterprojectupgrader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_converterprojectupgrader COMMAND test_converterprojectupgrader)

# Converter Tool
add_executable(test_convertertool testconvertertool.cpp)
target_link_libraries(test_convertertool gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_convertertool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_convertertool COMMAND test_convertertool)

# Converter Tool
add_executable(test_convertereditor testconvertereditor.cpp)
target_link_libraries(test_convertereditor gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_convertereditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_convertereditor COMMAND test_convertereditor)
