find_library(DISCOUNT_LIBRARY markdown)
find_library(CMARK_GFM_LIBRARY cmark-gfm)
find_library(CMARK_GFM_EXTENSIONS_LIBRARY cmark-gfm-extensions)

# Check if discount was found
if (NOT DISCOUNT_LIBRARY)

  if (NOT CMARK_GFM_LIBRARY OR NOT CMARK_GFM_EXTENSIONS_LIBRARY)
    message(FATAL_ERROR "Could NOT find DISCOUNT (libmarkdown)")
  endif()

  # Check cmark-gfm
  if (NOT CMARK_GFM_LIBRARY)
    message(FATAL_ERROR "Could NOT find CMARK-GFM")
  else()
    #message(STATUS "Found CMARK-GFM: ${CMARK_GFM_LIBRARY}")
    set (MARKDOWN_LIBRARY ${CMARK_GFM_LIBRARY})
    set (MARKDOWN_LIBRARY_NAME "MARKDOWN_CMARK_GFM")
  endif()

  # Check cmark-gfm-extensions
  if (NOT CMARK_GFM_EXTENSIONS_LIBRARY)
    message(FATAL_ERROR "Could NOT find CMARK-GFM-EXTENSIONS")
  else()
    #message(STATUS "Found CMARK-GFM-EXTENSIONS: ${CMARK_GFM_EXTENSIONS_LIBRARY}")
    set (MARKDOWN_LIBRARY "${MARKDOWN_LIBRARY} ${CMARK_GFM_LIBRARY}")
  endif()

else()
  set (MARKDOWN_LIBRARY ${DISCOUNT_LIBRARY})
  set (MARKDOWN_LIBRARY_NAME "MARKDOWN_DISCOUNT")
endif()

if (NOT MARKDOWN_LIBRARY)
  message(FATAL_ERROR "Could NOT find a MARKDOWN library. Please install discount (libmarkdown) or cmark-gfm!")
else()
  message(STATUS "Found MARKDOWN: ${MARKDOWN_LIBRARY}")
endif()



