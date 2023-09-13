cmake_minimum_required(VERSION 3.19)

find_library(GM-COMPANION-LIB_DIR gm-companion-lib)

add_subdirectory(ui)

set(SOURCES
  main.cpp
  logger.h
  logger.cpp
  version.h
  version.h.in
)

set(RCFILES
  gm-companion.rc
)

qt_add_executable(gm-companion ${SOURCES} ${RCFILES})

if (BUILD_TRANSLATIONS)
  file(GLOB_RECURSE TO_TRANSLATE
    LIST_DIRECTORIES false
    ${CMAKE_SOURCE_DIR}/app/**/*.cpp
    ${CMAKE_SOURCE_DIR}/app/**/*.h
    ${CMAKE_SOURCE_DIR}/app/**/*.qml
    ${CMAKE_SOURCE_DIR}/app/**/*.js
    ${CMAKE_SOURCE_DIR}/src/**/*.cpp
    ${CMAKE_SOURCE_DIR}/src/**/*.h
    ${CMAKE_SOURCE_DIR}/src/**/*.qml
    ${CMAKE_SOURCE_DIR}/src/**/*.js
  )

  qt_add_translations(gm-companion
    TS_FILES
    i18n/gm-companion_de_DE.ts
    i18n/gm-companion_en_US.ts
    SOURCES ${TO_TRANSLATE}
    LUPDATE_OPTIONS "-no-obsolete"
  )
endif()

target_precompile_headers(gm-companion REUSE_FROM common)

target_include_directories(gm-companion PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(gm-companion PRIVATE
  gm-companion-lib
  gm-companion-ui
  gm-companion-uiplugin
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickControls2
  Qt6::Qml
  sentry
)

set_target_properties(gm-companion PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

install(
  TARGETS gm-companion
  EXPORT companion
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/lol.rophil.gm_companion.desktop
  DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/gm-companion.png
  DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps/
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../misc/icons/hicolor
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/lol.rophil.gm_companion.appdata.xml
  DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/
)
