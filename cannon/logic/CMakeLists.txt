list(APPEND LOGIC_SOURCES 
  form.cpp
  )

add_library(logic ${LOGIC_SOURCES})
target_link_libraries(logic ${LIBRARIES})
