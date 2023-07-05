cmake_minimum_required(VERSION 3.1.0)

find_package(Qt6 REQUIRED COMPONENTS Core Quick)

set(FONT_AWESOME_DIR ${EXTERNAL_LIB_DIR}/fontawesome.pri)

if(NOT EXISTS ${FONT_AWESOME_DIR}/fontawesome.qrc)
    message(FATAL_ERROR "The FontAwesome submodule was not downloaded! Please run  git submodules update --init --recursive  and try again.")
endif()

set(SOURCES
  ${EXTERNAL_LIB_DIR}/fontawesome.pri/fontawesome.qrc
)

add_library(fontawesome STATIC ${SOURCES})
target_link_libraries(fontawesome Qt6::Core Qt6::Quick)
