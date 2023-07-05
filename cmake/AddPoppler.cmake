find_library(POPPLER_QT6_LIBRARY poppler-qt6)

if (NOT POPPLER_QT6_LIBRARY)
  message(FATAL_ERROR "Could NOT find POPPLER-QT6")
else()
  message(STATUS "Found POPPLER-QT6: ${POPPLER_QT6_LIBRARY}")
endif()

