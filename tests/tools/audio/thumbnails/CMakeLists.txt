cmake_minimum_required(VERSION 3.19)

add_subdirectory(loaders)

# thumbnail
qt_add_executable(test_audiothumbnail testaudiothumbnail.cpp)
target_link_libraries(test_audiothumbnail PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_audiothumbnail PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audiothumbnail COMMAND test_audiothumbnail)
