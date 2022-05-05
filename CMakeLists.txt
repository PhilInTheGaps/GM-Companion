cmake_minimum_required(VERSION 3.1.0)

project(gm-companion)

include(GNUInstallDirs)

set(CGET_TOOLCHAIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/cget/cget/cget.cmake)

if (EXISTS ${CGET_TOOLCHAIN_FILE})
  include(${CGET_TOOLCHAIN_FILE})
  message(STATUS "Found CGet toolchain file: ${CGET_TOOLCHAIN_FILE}")
endif()

# set c++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# set version number
include(SetVersionNumber)

message(STATUS "Build directory: ${CMAKE_CURRENT_BINARY_DIR}")

enable_testing(true)

include(AddAsyncFuture)
include(AddPropertyHelper)
include(AddHttpStatusCodes)

add_subdirectory(thirdparty/sentry-native)

# Code Coverage
if (ENABLE_CODE_COVERAGE)
  include(CodeCoverage)

  # turn off optimization for non-skewed coverage reports
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

  append_coverage_compiler_flags()
endif()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)
