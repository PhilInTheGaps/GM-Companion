cmake_minimum_required(VERSION 3.1.0)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Network Test)

set(BASE_FILES
  abstractaccesstest.h
  abstractaccesstest.cpp
)

# Local System
add_executable(test_localaccess ${BASE_FILES} testlocalaccess.cpp)
target_link_libraries(test_localaccess testhelper filesystem Qt5::Core Qt5::Test)
set_target_properties(test_localaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_localaccess COMMAND test_localaccess)

# File Dialog
add_executable(test_filedialog testfiledialog.cpp)
target_link_libraries(test_filedialog testhelper filesystem Qt5::Core Qt5::Test)
set_target_properties(test_filedialog PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_filedialog COMMAND test_filedialog)

if (ENABLE_EXTRA_TESTS)
    # Nextcloud
    add_executable(test_nextcloudaccess ${BASE_FILES} testnextcloudaccess.cpp)
    target_link_libraries(test_nextcloudaccess testhelper filesystem Qt5::Core Qt5::Network Qt5::Test)
    set_target_properties(test_nectcloudaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
    add_test(NAME test_nextcloudaccess COMMAND test_nextcloudaccess)

    # GoogleDrive
    add_executable(test_googledriveaccess ${BASE_FILES} testgoogledriveaccess.cpp)
    target_link_libraries(test_googledriveaccess testhelper filesystem Qt5::Core Qt5::Network Qt5::Test)
    set_target_properties(test_googledriveaccess PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
    add_test(NAME test_googledriveaccess COMMAND test_googledriveaccess)
endif()
