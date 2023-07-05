cmake_minimum_required(VERSION 3.10)

qt_add_library(http-status-codes INTERFACE)

target_include_directories(http-status-codes INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

set_target_properties(http-status-codes PROPERTIES PUBLIC_HEADER "thirdparty/http-status-codes/HttpStatusCodes_Qt.h")

install(TARGETS http-status-codes
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include)
