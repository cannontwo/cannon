list(APPEND LOG_SOURCES 
  registry.cpp
  logger.cpp
  level.cpp
  )

add_library(log ${LOG_SOURCES})
target_link_libraries(log ${LIBRARIES})
