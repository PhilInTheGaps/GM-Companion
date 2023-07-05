cmake_minimum_required(VERSION 3.19)

# Project Upgrader
qt_add_executable(test_converterprojectupgrader testconverterprojectupgrader.cpp)
target_link_libraries(test_converterprojectupgrader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_converterprojectupgrader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_converterprojectupgrader COMMAND test_converterprojectupgrader)

# Converter Tool
qt_add_executable(test_convertertool testconvertertool.cpp)
target_link_libraries(test_convertertool PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_convertertool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_convertertool COMMAND test_convertertool)

# Converter Tool
qt_add_executable(test_convertereditor testconvertereditor.cpp)
target_link_libraries(test_convertereditor PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_convertereditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_convertereditor COMMAND test_convertereditor)
