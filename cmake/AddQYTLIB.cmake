cmake_minimum_required(VERSION 3.1.0)

if(NOT EXISTS ${EXTERNAL_LIB_DIR}/qytlib/README.md)
    message(FATAL_ERROR "The QYTLIB submodule was not downloaded! Please run  git submodules update --init --recursive  and try again.")
endif()

add_subdirectory(${EXTERNAL_LIB_DIR}/qytlib ${EXTERNAL_LIB_DIR}/qytlib)
