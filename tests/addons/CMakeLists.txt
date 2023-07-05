cmake_minimum_required(VERSION 3.19)

# Addon Manager
qt_add_executable(test_addonmanager testaddonmanager.cpp)
target_link_libraries(test_addonmanager PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_addonmanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_addonmanager COMMAND test_addonmanager)

# Addon Reader
qt_add_executable(test_addonreader testaddonreader.cpp)
target_link_libraries(test_addonreader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_addonreader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_addonreader COMMAND test_addonreader)
