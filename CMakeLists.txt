cmake_minimum_required(VERSION 3.26)

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

# put all executables into one directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

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

enable_testing()

# Code Coverage
if (ENABLE_CODE_COVERAGE)
  include(CodeCoverage)

  # turn off optimization for non-skewed coverage reports
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

  append_coverage_compiler_flags()
endif()

include(AddPoppler)
include(AddTagLib)
include(AddGSL)
include(AddPropertyHelper)
include(AddHttpStatusCodes)
include(AddMarkdownLib)
include(AddO2)

# Disable Unity Builds for sentry and gtest
set(UNITY_BUILD_TEMP ${CMAKE_UNITY_BUILD})
set(CMAKE_UNITY_BUILD OFF)

set(SENTRY_ENABLE_INSTALL ON)
add_subdirectory(thirdparty/sentry-native)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(thirdparty/googletest)

# Re-enable Unity Builds
set(CMAKE_UNITY_BUILD ${UNITY_BUILD_TEMP})

add_subdirectory(thirdparty/qml-icon-fonts)

if (WIN32)
  add_compile_definitions(WIN32_LEAN_AND_MEAN NOMINMAX)

  # copy dlls to bin directory
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CGET_PREFIX}/bin ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CGET_PREFIX}/lib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
endif()

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)

feature_summary(WHAT ALL)
