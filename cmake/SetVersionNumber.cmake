set (APP_VERSION_MAJOR    1)
set (APP_VERSION_MINOR    2)
set (APP_VERSION_PATCH    0)
set (APP_VERSION_REVISION 0)
set (APP_VERSION_TITLE    "Release 1.2")

# Windows header
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/app/version.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/app/version.h
  @ONLY
)

# UpdateManager
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/updates/version.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/updates/version.h
  @ONLY
)

message(STATUS "GM-Companion Version: ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")
