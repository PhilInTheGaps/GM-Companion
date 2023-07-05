cmake_minimum_required(VERSION 3.19)

# UpdateManager
qt_add_executable(test_updatemanager testupdatemanager.cpp)
target_link_libraries(test_updatemanager PRIVATE
    common
    qtasyncfuture
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test)
set_target_properties(test_updatemanager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_updatemanager COMMAND test_updatemanager)
