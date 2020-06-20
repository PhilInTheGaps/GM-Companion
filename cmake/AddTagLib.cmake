find_library(TAG_LIBRARY tag)

if (NOT TAG_LIBRARY)
  message(FATAL_ERROR "Could NOT find TAGLIB")
else()
  message(STATUS "Found TAGLIB: ${TAG_LIBRARY}")
endif()

