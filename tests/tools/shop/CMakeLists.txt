cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)

# Shop Project and Item Group
add_executable(test_shopproject testshopproject.cpp)
target_link_libraries(test_shopproject gm-companion-lib common testhelper Qt5::Test)
set_target_properties(test_shopproject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shopproject COMMAND test_shopproject)

# Shop Tool
add_executable(test_shoptool testshoptool.cpp)
target_link_libraries(test_shoptool gm-companion-lib common filesystem services testhelper Qt5::Gui Qt5::Quick Qt5::Test)
set_target_properties(test_shoptool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shoptool COMMAND test_shoptool)

# Shop Editor
add_executable(test_shopeditor testshopeditor.cpp)
target_link_libraries(test_shopeditor gm-companion-lib common filesystem services testhelper Qt5::Gui Qt5::Quick Qt5::Test)
set_target_properties(test_shopeditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shopeditor COMMAND test_shopeditor)
