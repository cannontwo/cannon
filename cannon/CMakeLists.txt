list(APPEND CANNON_SOURCES
  null.cpp
  )

add_subdirectory(math)
add_subdirectory(log)
add_subdirectory(logic)
add_subdirectory(graphics)

add_library(cannon ${CANNON_SOURCES})
target_link_libraries(cannon 
  math 
  log 
  logic
  graphics
  ${LIBRARIES})

# Copy shaders
ADD_CUSTOM_COMMAND(
  TARGET cannon POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/shaders ${CMAKE_CURRENT_BINARY_DIR}/../shaders
  COMMENT "Copying shaders" VERBATIM
  )

# Copy assets
ADD_CUSTOM_COMMAND(
  TARGET cannon POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics/assets ${CMAKE_CURRENT_BINARY_DIR}/../assets
  COMMENT "Copying assets" VERBATIM
  )
