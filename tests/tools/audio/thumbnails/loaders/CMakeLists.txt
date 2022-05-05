cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Network Test)

# tag loader
add_executable(test_tagimageloader testtagimageloader.cpp)
target_link_libraries(test_tagimageloader gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_tagimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_tagimageloader COMMAND test_tagimageloader)

# file loader
add_executable(test_fileimageloader testfileimageloader.cpp)
target_link_libraries(test_fileimageloader gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_fileimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_fileimageloader COMMAND test_fileimageloader)

# web loader
# somehow this always fails using github actions (windows) even though the compiled executable works on other machines
# so we disable this test for now
if(NOT (IS_RUNNING_IN_CI AND WIN32))
    add_executable(test_webimageloader testwebimageloader.cpp)
    target_link_libraries(test_webimageloader gm-companion-lib common testhelper Qt5::Gui Qt5::Network Qt5::Test)
    set_target_properties(test_webimageloader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
    add_test(NAME test_webimageloader COMMAND test_webimageloader)
endif()
