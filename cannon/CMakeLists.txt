list(APPEND CANNON_SOURCES
  fake.cpp
  )

add_subdirectory(math)

add_library(cannon ${CANNON_SOURCES})
target_link_libraries(cannon math ${LIBRARIES})
