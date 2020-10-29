cmake_minimum_required(VERSION 3.10)
project(Cannon)

# Compilation configuration
ADD_DEFINITIONS(
  -Wall
  -g
  -O2
)

ENABLE_TESTING()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

# Produce code coverage target
if(CMAKE_COMPILER_IS_GNUCXX)
  include(CodeCoverage)
  append_coverage_compiler_flags()
  setup_target_for_coverage_gcovr_html(NAME ${PROJECT_NAME}_coverage ${PROJECT_TEST_NAME} coverage EXCLUDE ${PROJECT_SOURCE_DIR}/tests EXCLUDE ${PROJECT_SOURCE_DIR}/cannon/graphics EXCLUDE ${PROJECT_SOURCE_DIR}/scripts EXCLUDE ${PROJECT_SOURCE_DIR}/cannon/plot EXCLUDE ${PROJECT_SOURCE_DIR}/thirdparty)
endif()

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

set(CMAKE_CXX_STANDARD 11)

# Clang Tidy
find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-format-style=file;-header-filter='${CMAKE_SOURCE_DIR}/*'" CACHE STRING "" FORCE)
else()
    message(STATUS "clang-tidy NOT found!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # clear it
endif()

# OMPL
find_package(OMPL REQUIRED)
include_directories(${OMPL_INCLUDE_DIRS})
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})
list(APPEND LIBRARIES ${OMPL_LIBRARIES})

# Eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# CGAL
find_package(CGAL REQUIRED)
include_directories(${CGAL_INCLUDE_DIRS})
list(APPEND LIBRARIES ${CGAL_LIBRARIES})

# OpenGL
find_package(OpenGL 4.3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
list(APPEND LIBRARIES ${OPENGL_LIBRARIES})

# GLAD
add_library(GLAD "thirdparty/glad/src/glad.c")
target_link_libraries(GLAD ${CMAKE_DL_LIBS})
include_directories("thirdparty/glad/include")
list(APPEND LIBRARIES GLAD)

# STB_IMAGE
add_library(STB_IMAGE "thirdparty/stb_image/stb_image.cpp")
include_directories("thirdparty/stb_image/include")
list(APPEND LIBRARIES STB_IMAGE)

# GLFW
find_package(GLFW3 REQUIRED)
include_directories(${GLFW3_INCLUDE_DIR})
list(APPEND LIBRARIES ${GLFW3_LIBRARY})

# FreeType
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
list(APPEND LIBRARIES ${FREETYPE_LIBRARIES})

# Boost
find_package(Boost 1.74.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

## Source code
include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory(cannon)

# Find unit tests
file( GLOB_RECURSE TEST_SOURCES cannon/*.test.cpp )
foreach (test_source ${TEST_SOURCES})
  get_filename_component( testname ${test_source} NAME_WE)
  add_executable( ${testname} ${test_source} )
  target_link_libraries( ${testname} cannon ${LIBRARIES})
  set_target_properties(${testname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/unit_tests")
  add_test(${testname} "unit_tests/${testname}")
endforeach(test_source  ${TEST_SOURCES})

# Find scripts
file( GLOB_RECURSE SCRIPT_SOURCES scripts/*.cpp )
foreach (script_source ${SCRIPT_SOURCES})
  get_filename_component( scriptname ${script_source} NAME_WE)
  add_executable( ${scriptname} ${script_source} )
  target_link_libraries( ${scriptname} cannon ${LIBRARIES})
  target_include_directories( ${scriptname} PUBLIC "${PROJECT_SOURCE_DIR}")
  set_target_properties(${scriptname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/scripts")
endforeach (script_source ${SCRIPT_SOURCES})
