list(APPEND CANNON_SOURCES
  null.cpp
  )

add_subdirectory(math)
add_subdirectory(log)
add_subdirectory(logic)
add_subdirectory(graphics)
add_subdirectory(plot)
add_subdirectory(ml)
add_subdirectory(control)
add_subdirectory(physics)
add_subdirectory(geom)
add_subdirectory(research)

add_library(cannon SHARED ${CANNON_SOURCES}
  $<TARGET_OBJECTS:math>
  $<TARGET_OBJECTS:log>
  $<TARGET_OBJECTS:logic>
  $<TARGET_OBJECTS:graphics>
  $<TARGET_OBJECTS:plot>
  $<TARGET_OBJECTS:ml>
  $<TARGET_OBJECTS:control>
  $<TARGET_OBJECTS:physics>
  $<TARGET_OBJECTS:geom>
  $<TARGET_OBJECTS:research>
  )
target_link_libraries(cannon ${LIBRARIES})

# Copy CNF formulas
ADD_CUSTOM_COMMAND(
  TARGET cannon POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/logic/formulas ${CMAKE_CURRENT_BINARY_DIR}/../formulas
  COMMENT "Copying formulas" VERBATIM
  )

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
