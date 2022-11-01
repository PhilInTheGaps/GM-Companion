cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Gui Quick Test)


# Audio Tool
add_subdirectory(audio)

# Combat Tracker
add_executable(test_combattracker testcombattracker.cpp ${RESOURCES})
target_link_libraries(test_combattracker gm-companion-lib common Qt5::Gui Qt5::Quick Qt5::Test)
set_target_properties(test_combattracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_combattracker COMMAND test_combattracker)

# Name Generator
add_executable(test_namegenerator testnamegenerator.cpp ${RESOURCES})
target_link_libraries(test_namegenerator gm-companion-lib common testhelper Qt5::Gui Qt5::Quick Qt5::Test)
set_target_properties(test_namegenerator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_namegenerator COMMAND test_namegenerator)
