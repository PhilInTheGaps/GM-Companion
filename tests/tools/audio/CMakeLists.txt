cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)

add_subdirectory(thumbnails)

# Audio Project
add_executable(test_audioproject testaudioproject.cpp)
target_link_libraries(test_audioproject gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_audioproject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audioproject COMMAND test_audioproject)

# Audio SaveLoad
add_executable(test_audiosaveload testaudiosaveload.cpp)
target_link_libraries(test_audiosaveload gm-companion-lib common testhelper Qt5::Gui Qt5::Test)
set_target_properties(test_audiosaveload PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_audiosaveload COMMAND test_audiosaveload)
