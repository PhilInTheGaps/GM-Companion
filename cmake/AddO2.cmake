cmake_minimum_required(VERSION 3.1.0)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)

# check if submodules are updated
if(NOT EXISTS ${EXTERNAL_LIB_DIR}/o2/CMakeLists.txt)
    message(FATAL_ERROR "The O2 submodule was not downloaded! Please run  git submodules update --init --recursive  and try again.")
endif()

# find qt5 packages
find_package(Qt5 REQUIRED COMPONENTS Core Network)
find_package(QtKeychain)

# o2 lib
set(o2_SOURCES
  ${EXTERNAL_LIB_DIR}/o2/src/o2.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2reply.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2replyserver.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2requestor.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2simplecrypt.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o0settingsstore.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o0baseauth.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2google.cpp
  ${EXTERNAL_LIB_DIR}/o2/src/o2spotify.cpp
)

set(o2_HEADERS
  ${EXTERNAL_LIB_DIR}/o2/src/o2.h
  ${EXTERNAL_LIB_DIR}/o2/src/o2reply.h
  ${EXTERNAL_LIB_DIR}/o2/src/o2replyserver.h
  ${EXTERNAL_LIB_DIR}/o2/src/o2requestor.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0abstractstore.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0baseauth.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0export.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0globals.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0requestparameter.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0settingsstore.h
  ${EXTERNAL_LIB_DIR}/o2/src/o0simplecrypt.h
  ${EXTERNAL_LIB_DIR}/o2/src/o2google.h
  ${EXTERNAL_LIB_DIR}/o2/src/o2spotify.h
)

if(QTKEYCHAIN_FOUND)
  list(APPEND LINK_TARGETS ${QTKEYCHAIN_LIBRARY})
  include_directories(${QTKEYCHAIN_INCLUDE_DIR})
  set( o2_SOURCES
    ${o2_SOURCES}
    ${EXTERNAL_LIB_DIR}/o2/src/o0keychainstore.cpp
  )
  set( o2_HEADERS
    ${o2_HEADERS}
    ${EXTERNAL_LIB_DIR}/o2/src/o0keychainstore.h
  )
else()
  message(FATAL_ERROR "Qt5Keychain or QtKeychain is required")
endif()

add_library(o2 STATIC ${o2_SOURCES} ${o2_HEADERS})
target_link_libraries(o2 Qt5::Core Qt5::Network ${LINK_TARGETS})
target_include_directories(o2 PUBLIC ${EXTERNAL_LIB_DIR}/o2/src)

# needed for statically linked o2 in shared libs on x86_64
set_target_properties(o2
  PROPERTIES POSITION_INDEPENDENT_CODE TRUE
)

install(FILES ${o2_HEADERS}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/o2
)

install(TARGETS o2
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib${o2_LIB_SUFFIX}
  ARCHIVE DESTINATION lib${o2_LIB_SUFFIX}
)
