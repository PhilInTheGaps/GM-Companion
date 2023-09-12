cmake_minimum_required(VERSION 3.19)

set(BASE_FILES
  abstractaccesstest.h
  abstractaccesstest.cpp
)

# Local System
qt_add_executable(test_localaccess ${BASE_FILES} testlocalaccess.cpp)
target_link_libraries(test_localaccess PRIVATE testhelper filesystem Qt6::Core Qt6::Test)
set_target_properties(test_localaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_localaccess COMMAND test_localaccess)

# File Dialog
qt_add_executable(test_filedialog testfiledialog.cpp)
target_link_libraries(test_filedialog PRIVATE testhelper filesystem gm-companion-ui-filedialog gm-companion-ui-filedialogplugin Qt6::Core Qt6::Test)
set_target_properties(test_filedialog PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_filedialog COMMAND test_filedialog)

# Nextcloud
qt_add_executable(test_nextcloudaccess ${BASE_FILES} testnextcloudaccess.cpp mocknextcloud.cpp mocknextcloud.h oputils.h oputils.cpp)
target_link_libraries(test_nextcloudaccess PRIVATE testhelper filesystem Qt6::Core Qt6::Network Qt6::Test)
set_target_properties(test_nextcloudaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_nextcloudaccess COMMAND test_nextcloudaccess)

if (ENABLE_EXTRA_TESTS)
    # GoogleDrive
    qt_add_executable(test_googledriveaccess ${BASE_FILES} testgoogledriveaccess.cpp)
    target_link_libraries(test_googledriveaccess PRIVATE testhelper filesystem Qt6::Core Qt6::Network Qt6::Test)
    set_target_properties(test_googledriveaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
    add_test(NAME test_googledriveaccess COMMAND test_googledriveaccess)
endif()
