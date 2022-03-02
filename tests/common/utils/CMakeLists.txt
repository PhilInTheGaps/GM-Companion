cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Network Test)

# Utils
add_executable(test_utils testutils.cpp)
target_link_libraries(test_utils common Qt5::Gui Qt5::Test)
set_target_properties(test_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_utils COMMAND test_utils)

# File Utils
add_executable(test_fileutils testfileutils.cpp)
target_link_libraries(test_fileutils common Qt5::Test)
set_target_properties(test_fileutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_fileutils COMMAND test_fileutils)

# Network Utils
add_executable(test_networkutils testnetworkutils.cpp)
target_link_libraries(test_networkutils common Qt5::Test Qt5::Network)
set_target_properties(test_networkutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_networkutils COMMAND test_networkutils)

# Markdown Utils
add_executable(test_markdownutils testmarkdownutils.cpp)
target_link_libraries(test_markdownutils common Qt5::Test)
set_target_properties(test_markdownutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_markdownutils COMMAND test_markdownutils)

# Process Info
add_executable(test_processinfo testprocessinfo.cpp)
target_link_libraries(test_processinfo common Qt5::Test)
set_target_properties(test_processinfo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_processinfo COMMAND test_processinfo)
