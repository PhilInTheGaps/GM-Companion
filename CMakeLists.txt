cmake_minimum_required(VERSION 3.19)

project(gm-companion LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(FeatureSummary)

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

find_package(Qt6 COMPONENTS Core Gui Quick QuickControls2 Qml Multimedia Network DBus Concurrent Core5Compat LinguistTools Test REQUIRED)
find_package(QuaZip-Qt6 REQUIRED)
find_package(QtKeychain REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)
set(CMAKE_AUTORCC ON) # turn on automatic invocation of the RCC
add_compile_definitions(QT_DEPRECATED_WARNINGS)

# set version number
include(SetVersionNumber)

# build options
option(BUILD_TRANSLATIONS "Generate translation files" ON)
cmake_print_variables(BUILD_TRANSLATIONS)
option(ENABLE_CODE_COVERAGE "Generate code coverage, don't use this for regular builds" OFF)
cmake_print_variables(ENABLE_CODE_COVERAGE)
option(ENABLE_EXTRA_TESTS "Enable extra tests that require special setup and can not be run on every machine" OFF)
cmake_print_variables(ENABLE_EXTRA_TESTS)
option(IS_RUNNING_IN_CI "Variable used to en- and disable stuff when running in CI environments" OFF)
cmake_print_variables(IS_RUNNING_IN_CI)

message(STATUS "Build directory: ${CMAKE_CURRENT_BINARY_DIR}")

enable_testing(true)

include(AddPoppler)
include(AddTagLib)
include(AddGSL)
include(AddPropertyHelper)
include(AddHttpStatusCodes)
include(AddMarkdownLib)
include(AddO2)

set(SENTRY_ENABLE_INSTALL ON)
add_subdirectory(thirdparty/sentry-native)

add_subdirectory(thirdparty/qml-icon-fonts)

# Code Coverage
if (ENABLE_CODE_COVERAGE)
  include(CodeCoverage)

  # turn off optimization for non-skewed coverage reports
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

  append_coverage_compiler_flags()
endif()

if (WIN32)
  add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)

feature_summary(WHAT ALL)
