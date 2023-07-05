cmake_minimum_required(VERSION 3.10)

qt_add_library(qtasyncfuture INTERFACE)

target_include_directories(qtasyncfuture INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

set_target_properties(qtasyncfuture PROPERTIES PUBLIC_HEADER "thirdparty/asyncfuture/asyncfuture.h")

install(TARGETS qtasyncfuture
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include)
