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
add_test(NAME test_localaccess COMMAND test_localaccess)

# Nextcloud
if (ENABLE_EXTRA_TESTS)
    add_executable(test_nextcloudaccess ${BASE_FILES} testnextcloudaccess.cpp)
    target_link_libraries(test_nextcloudaccess testhelper filesystem Qt5::Core Qt5::Network Qt5::Test)
    add_test(NAME test_nextcloudaccess COMMAND test_nextcloudaccess)
endif()

# File Dialog
add_executable(test_filedialog testfiledialog.cpp)
target_link_libraries(test_filedialog testhelper filesystem Qt5::Core Qt5::Test)
add_test(NAME test_filedialog COMMAND test_filedialog)
