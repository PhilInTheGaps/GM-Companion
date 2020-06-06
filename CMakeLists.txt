cmake_minimum_required(VERSION 3.1.0)

project(gm-companion)

include(GNUInstallDirs)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# set external lib dir
set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

enable_testing(true)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)
