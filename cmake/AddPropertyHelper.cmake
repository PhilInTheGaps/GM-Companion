cmake_minimum_required(VERSION 3.10)

qt_add_library(propertyhelper INTERFACE)

target_include_directories(propertyhelper INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

set_target_properties(propertyhelper PROPERTIES PUBLIC_HEADER "thirdparty/propertyhelper/PropertyHelper.h")

install(TARGETS propertyhelper
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include)
