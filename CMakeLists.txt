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
  setup_target_for_coverage_gcovr_html(NAME ${PROJECT_NAME}_coverage ${PROJECT_TEST_NAME} coverage EXCLUDE ${PROJECT_SOURCE_DIR}/tests)
endif()

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

set(CMAKE_CXX_STANDARD 11)

find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-format-style=file;-header-filter='${CMAKE_SOURCE_DIR}/*'" CACHE STRING "" FORCE)
else()
    message(STATUS "clang-tidy NOT found!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # clear it
endif()

# Packages
find_package(OMPL REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

# Targets
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/cannon/*.cpp)

include_directories(${OMPL_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})
link_directories(${OMPL_LIBRARY_DIRS})
add_definitions(${OMPL_DEFINITIONS})

add_library(cannon ${SOURCES})
target_link_libraries(cannon ${OMPL_LIBRARIES})
target_include_directories(cannon PUBLIC "${PROJECT_SOURCE_DIR}")

add_executable(compile_test tests/compile.test.cpp)
target_link_libraries(compile_test cannon)
target_include_directories(compile_test PUBLIC "${PROJECT_SOURCE_DIR}")

# Find unit tests
file( GLOB_RECURSE TEST_SOURCES cannon/*.test.cpp )
foreach (test_source ${TEST_SOURCES})
  get_filename_component( testname ${test_source} NAME_WE)
  add_executable( ${testname} ${test_source} )
  target_link_libraries( ${testname} cannon)
  add_test(${testname} ${testname})
endforeach(test_source  ${TEST_SOURCES})

