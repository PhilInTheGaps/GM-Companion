cmake_minimum_required(VERSION 3.19)

# Shop Project and Item Group
qt_add_executable(test_shopproject testshopproject.cpp)
target_link_libraries(test_shopproject PRIVATE gm-companion-lib common testhelper Qt6::Test)
set_target_properties(test_shopproject PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shopproject COMMAND test_shopproject)

# Shop Tool
qt_add_executable(test_shoptool testshoptool.cpp)
target_link_libraries(test_shoptool PRIVATE gm-companion-lib common filesystem services testhelper Qt6::Gui Qt6::Quick Qt6::Test)
set_target_properties(test_shoptool PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shoptool COMMAND test_shoptool)

# Shop Editor
qt_add_executable(test_shopeditor testshopeditor.cpp)
target_link_libraries(test_shopeditor PRIVATE gm-companion-lib common filesystem services testhelper Qt6::Gui Qt6::Quick Qt6::Test)
set_target_properties(test_shopeditor PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_shopeditor COMMAND test_shopeditor)
