cmake_minimum_required(VERSION 3.19)

# Audio Tool
add_subdirectory(audio)

# Converter Tool
add_subdirectory(converter)

# Shop Tool
add_subdirectory(shop)

# Combat Tracker
qt_add_executable(test_combattracker testcombattracker.cpp ${RESOURCES})
target_link_libraries(test_combattracker PRIVATE gm-companion-lib common Qt6::Gui Qt6::Quick Qt6::Test)
set_target_properties(test_combattracker PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_combattracker COMMAND test_combattracker)

# Name Generator
qt_add_executable(test_namegenerator testnamegenerator.cpp ${RESOURCES})
target_link_libraries(test_namegenerator PRIVATE gm-companion-lib common testhelper Qt6::Gui Qt6::Quick Qt6::Test)
set_target_properties(test_namegenerator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_namegenerator COMMAND test_namegenerator)
