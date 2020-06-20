find_library(POPPLER_QT5_LIBRARY poppler-qt5)

if (NOT POPPLER_QT5_LIBRARY)
  message(FATAL_ERROR "Could NOT find POPPLER-QT5")
else()
  message(STATUS "Found POPPLER-QT5: ${POPPLER_QT5_LIBRARY}")
endif()

