cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)

# Addon Manager
add_executable(test_addonmanager testaddonmanager.cpp)
target_link_libraries(test_addonmanager gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_addonmanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_addonmanager COMMAND test_addonmanager)

# Addon Reader
add_executable(test_addonreader testaddonreader.cpp)
target_link_libraries(test_addonreader gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_addonreader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_addonreader COMMAND test_addonreader)
