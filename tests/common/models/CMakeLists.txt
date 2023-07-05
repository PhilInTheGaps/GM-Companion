cmake_minimum_required(VERSION 3.19)

# Tree Item
qt_add_executable(test_treeitem testtreeitem.cpp)
target_link_libraries(test_treeitem PRIVATE propertyhelper common Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(test_treeitem PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_treeitem COMMAND test_treeitem)

# Custom Object List Model
qt_add_executable(test_customobjectlistmodel testcustomobjectlistmodel.cpp)
target_link_libraries(test_customobjectlistmodel PRIVATE propertyhelper common Qt6::Core Qt6::Gui Qt6::Test)
set_target_properties(test_customobjectlistmodel PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_customobjectlistmodel COMMAND test_customobjectlistmodel)
