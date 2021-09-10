list(APPEND UTILS_SOURCES
  statistics.cpp
  argparser.cpp
  )

add_library(utils OBJECT ${UTILS_SOURCES})
target_compile_options(utils PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
