cmake_minimum_required(VERSION 3.19)

add_subdirectory(thumbnails)

# Audio Project
qt_add_executable(test_audioproject testaudioproject.cpp)
target_link_libraries(test_audioproject PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_audioproject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audioproject COMMAND test_audioproject)

# Audio Project Upgrader
qt_add_executable(test_audioprojectupgrader testaudioprojectupgrader.cpp)
target_link_libraries(test_audioprojectupgrader PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_audioprojectupgrader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audioprojectupgrader COMMAND test_audioprojectupgrader)

# Audio SaveLoad
qt_add_executable(test_audiosaveload testaudiosaveload.cpp)
target_link_libraries(test_audiosaveload PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_audiosaveload PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audiosaveload COMMAND test_audiosaveload)

# Audio Addons
qt_add_executable(test_audioaddons testaudioaddons.cpp)
target_link_libraries(test_audioaddons PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Test)
set_target_properties(test_audioaddons PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audioaddons COMMAND test_audioaddons)
