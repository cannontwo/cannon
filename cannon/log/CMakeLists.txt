list(APPEND LOG_SOURCES 
  registry.cpp
  logger.cpp
  level.cpp
  )

add_library(log OBJECT ${LOG_SOURCES})
target_compile_options(log PUBLIC -Wextra -Werror)
