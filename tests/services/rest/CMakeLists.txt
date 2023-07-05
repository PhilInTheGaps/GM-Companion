cmake_minimum_required(VERSION 3.19)

# Callback Server
qt_add_executable(test_callbackserver testcallbackserver.cpp)
target_link_libraries(test_callbackserver PRIVATE services testhelper Qt6::Network Qt6::Test)
set_target_properties(test_callbackserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_callbackserver COMMAND test_callbackserver)
