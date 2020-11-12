list(APPEND LOG_SOURCES 
  registry.cpp
  logger.cpp
  level.cpp
  )

add_library(log OBJECT ${LOG_SOURCES})
