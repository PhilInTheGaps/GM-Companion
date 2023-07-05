cmake_minimum_required(VERSION 3.19)

# Utils
qt_add_executable(test_utils testutils.cpp)
target_link_libraries(test_utils PRIVATE common Qt6::Gui Qt6::Test)
set_target_properties(test_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_utils COMMAND test_utils)

# String Utils
qt_add_executable(test_stringutils teststringutils.cpp)
target_link_libraries(test_stringutils PRIVATE common Qt6::Gui Qt6::Test)
set_target_properties(test_stringutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_stringutils COMMAND test_stringutils)

# File Utils
qt_add_executable(test_fileutils testfileutils.cpp)
target_link_libraries(test_fileutils PRIVATE common Qt6::Test)
set_target_properties(test_fileutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_fileutils COMMAND test_fileutils)

# Network Utils
qt_add_executable(test_networkutils testnetworkutils.cpp)
target_link_libraries(test_networkutils PRIVATE common Qt6::Test Qt6::Network)
set_target_properties(test_networkutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_networkutils COMMAND test_networkutils)

# Markdown Utils
qt_add_executable(test_markdownutils testmarkdownutils.cpp)
target_link_libraries(test_markdownutils PRIVATE common Qt6::Test)
set_target_properties(test_markdownutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_markdownutils COMMAND test_markdownutils)

# Process Info
qt_add_executable(test_processinfo testprocessinfo.cpp)
target_link_libraries(test_processinfo PRIVATE common Qt6::Test)
set_target_properties(test_processinfo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_processinfo COMMAND test_processinfo)
