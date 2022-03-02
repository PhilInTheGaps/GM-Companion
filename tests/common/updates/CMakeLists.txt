cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Network Test)

# UpdateManager
add_executable(test_updatemanager testupdatemanager.cpp)
target_link_libraries(test_updatemanager
    common
    qtasyncfuture
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Test)
set_target_properties(test_updatemanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_updatemanager COMMAND test_updatemanager)
