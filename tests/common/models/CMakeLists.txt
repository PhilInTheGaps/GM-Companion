cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Test)

# Tree Item
add_executable(test_treeitem testtreeitem.cpp)
target_link_libraries(test_treeitem propertyhelper common Qt5::Core Qt5::Gui Qt5::Test)
set_target_properties(test_treeitem PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_treeitem COMMAND test_treeitem)

# Custom Object List Model
add_executable(test_customobjectlistmodel testcustomobjectlistmodel.cpp)
target_link_libraries(test_customobjectlistmodel propertyhelper common Qt5::Core Qt5::Gui Qt5::Test)
set_target_properties(test_customobjectlistmodel PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_customobjectlistmodel COMMAND test_customobjectlistmodel)
