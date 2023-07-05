cmake_minimum_required(VERSION 3.19)

# tag loader
qt_add_executable(test_tagimageloader testtagimageloader.cpp)
target_link_libraries(test_tagimageloader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_tagimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_tagimageloader COMMAND test_tagimageloader)

# file loader
qt_add_executable(test_fileimageloader testfileimageloader.cpp)
target_link_libraries(test_fileimageloader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_fileimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_fileimageloader COMMAND test_fileimageloader)

# web loader
# somehow this always fails using github actions (windows) even though the compiled executable works on other machines
# so we disable this test for now
if(NOT (IS_RUNNING_IN_CI AND WIN32))
    qt_add_executable(test_webimageloader testwebimageloader.cpp)
    target_link_libraries(test_webimageloader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Network Qt6::Test)
    set_target_properties(test_webimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
    add_test(NAME test_webimageloader COMMAND test_webimageloader)
endif()
