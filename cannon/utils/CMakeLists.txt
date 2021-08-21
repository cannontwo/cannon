list(APPEND UTILS_SOURCES
  statistics.cpp
  argparser.cpp
  )

add_library(utils OBJECT ${UTILS_SOURCES})
