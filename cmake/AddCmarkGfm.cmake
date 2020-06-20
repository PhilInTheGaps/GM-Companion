find_library(CMARK_GFM_LIBRARY cmark-gfm)
find_library(CMARK_GFM_EXTENSIONS_LIBRARY cmark-gfm-extensions)

if (NOT CMARK_GFM_LIBRARY)
  message(FATAL_ERROR "Could NOT find CMARK-GFM")
else()
  message(STATUS "Found CMARK-GFM: ${CMARK_GFM_LIBRARY}")
endif()

if (NOT CMARK_GFM_EXTENSIONS_LIBRARY)
  message(FATAL_ERROR "Could NOT find CMARK-GFM-EXTENSIONS")
else()
  message(STATUS "Found CMARK-GFM-EXTENSIONS: ${CMARK_GFM_EXTENSIONS_LIBRARY}")
endif()
