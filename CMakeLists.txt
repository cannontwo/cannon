cmake_minimum_required(VERSION 3.10)
project(Cannon C CXX)
enable_language(CXX)

# Options
option(CANNON_BUILD_GRAPHICS "Whether to build graphics portions of the project" ON)
if(CANNON_BUILD_GRAPHICS)
  add_definitions( -DCANNON_BUILD_GRAPHICS=1 )
endif()

option(CANNON_BUILD_RESEARCH "Whether to build research portions of the project (will not work unless you have access to my private research repo)" OFF)
if(CANNON_BUILD_RESEARCH)
  add_definitions( -DCANNON_BUILD_RESEARCH=1 )
endif()

option(CANNON_BUILD_LIBRARY "Whether to build anything at all" ON)

# Building Documentation
option(CANNON_BUILD_DOC "Build documentation" OFF)

option(CANNON_BUILD_SCRIPTS "Build scripts" ON)

# Compilation configuration
#ADD_DEFINITIONS(
#  -Wall
#  -g
#  -O0
#)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")

ENABLE_TESTING()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Produce code coverage target
#if (CANNON_BUILD_LIBRARY)
#  if(CMAKE_COMPILER_IS_GNUCXX)
#    include(CodeCoverage)
#    append_coverage_compiler_flags()
#    setup_target_for_coverage_gcovr_html(NAME ${PROJECT_NAME}_coverage ${PROJECT_TEST_NAME} coverage 
#      EXCLUDE ${PROJECT_SOURCE_DIR}/tests 
#      EXCLUDE ${PROJECT_SOURCE_DIR}/cannon/graphics 
#      EXCLUDE ${PROJECT_SOURCE_DIR}/cannon/plot 
#      EXCLUDE ${PROJECT_SOURCE_DIR}/cannon/physics/systems
#      EXCLUDE ${PROJECT_SOURCE_DIR}/scripts
#      EXCLUDE ${PROJECT_SOURCE_DIR}/thirdparty)
#  endif()
#endif()

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
  EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
  )
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CGAL_EIGEN3_ENABLED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Profiling
#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# check if Doxygen is installed
if (CANNON_BUILD_DOC)

  # Doxygen
  find_package(Doxygen REQUIRED)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")

  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen")

  # note the option ALL which allows to build the docs together with the application
  add_custom_target( doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )

endif (CANNON_BUILD_DOC)


if (CANNON_BUILD_LIBRARY)
  # Clang Tidy
  find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
  if(CLANG_TIDY_EXE)
      set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-format-style=file;-header-filter='${CMAKE_SOURCE_DIR}/*'" CACHE STRING "" FORCE)
  else()
      message(STATUS "clang-tidy NOT found!")
      set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # clear it
  endif()

  # Threading
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)
  list(APPEND LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

  # OMPL
  # TODO Fix this
  set(OMPL_DIR "/home/cannon/Documents/ompl")
  find_package(ompl REQUIRED)
  include_directories(${OMPL_INCLUDE_DIRS})
  link_directories(${OMPL_LIBRARY_DIRS})
  add_definitions(${OMPL_DEFINITIONS})
  list(APPEND LIBRARIES ${OMPL_LIBRARIES})

  # Eigen
  find_package(Eigen3 3.3.4 REQUIRED)
  include_directories(${EIGEN3_INCLUDE_DIR})

  # CGAL
  find_package(CGAL REQUIRED COMPONENTS Core)
  include_directories(${CGAL_INCLUDE_DIRS})
  #list(APPEND LIBRARIES ${CGAL_LIBRARY})
  list(APPEND LIBRARIES CGAL::CGAL CGAL::CGAL_Core)

  # Boost
  find_package(Boost 1.74.0 REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})

  # yaml-cpp
  find_package(yaml-cpp 0.6.0 REQUIRED)
  include_directories(${YAML_CPP_INCLUDE_DIR})
  list(APPEND LIBRARIES ${YAML_CPP_LIBRARIES})

  # Catch2
  find_package(Catch2 REQUIRED)
  include(Catch)

  # Gurobi
  find_package(GUROBI)
  if (GUROBI_FOUND)
    add_definitions( -DCANNON_HAVE_GUROBI=1 )
    include_directories(${GUROBI_INCLUDE_DIRS})
    # A hack, for some reason findGUROBI isn't working
    list(APPEND LIBRARIES ${GUROBI_LIBRARY} ${GUROBI_CXX_LIBRARY})
  endif()

  # HDF5
  find_package(HDF5 REQUIRED)
  include_directories(${HDF5_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${HDF5_LIBRARIES})

  # HighFive
  set(HIGHFIVE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
  set(HIGHFIVE_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(HIGHFIVE_BUILD_DOCS OFF)
  set(HIGHFIVE_USE_EIGEN ON)
  add_subdirectory("thirdparty/HighFive")
  list(APPEND LIBRARIES HighFive)

  # TinyColorMap
  include_directories("thirdparty/tinycolormap/include")

  # STB_IMAGE
  add_library(STB_IMAGE "thirdparty/stb_image/stb_image.cpp")
  include_directories("thirdparty/stb_image/include")
  list(APPEND LIBRARIES STB_IMAGE)

  # Assimp
  find_package(assimp REQUIRED)
  include_directories(${ASSIMP_INCLUDE_DIRS})
  list(APPEND LIBRARIES ${ASSIMP_LIBRARIES})

  if (CANNON_BUILD_GRAPHICS)
    # OpenGL
    find_package(OpenGL 4.3 REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS})
    list(APPEND LIBRARIES ${OPENGL_LIBRARIES})

    # GLAD
    add_library(GLAD "thirdparty/glad/src/glad.c")
    target_link_libraries(GLAD ${CMAKE_DL_LIBS})
    include_directories("thirdparty/glad/include")
    list(APPEND LIBRARIES GLAD)

    # GLFW
    find_package(GLFW3 REQUIRED)
    include_directories(${GLFW3_INCLUDE_DIR})
    list(APPEND LIBRARIES ${GLFW3_LIBRARY})

    # FreeType
    find_package(Freetype REQUIRED)
    include_directories(${FREETYPE_INCLUDE_DIRS})
    list(APPEND LIBRARIES ${FREETYPE_LIBRARIES})

    # ImGui
    add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/imgui)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/imgui/backends)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparty/imgui/misc/cpp)
    list(APPEND IMGUI_SOURCES
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imconfig.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/misc/cpp/imgui_stdlib.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/misc/cpp/imgui_stdlib.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imgui_internal.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imstb_rectpack.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imstb_textedit.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/imstb_truetype.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_glfw.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_glfw.h
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl3.cpp
      ${PROJECT_SOURCE_DIR}/thirdparty/imgui/backends/imgui_impl_opengl3.h
      )
    add_library(imgui ${IMGUI_SOURCES})
    target_link_libraries(imgui ${GLFW3_LIBRARY} ${OPENGL_LIBRARIES})
    list(APPEND LIBRARIES imgui)

    # ImPlot
    #include_directories(${PROJECT_SOURCE_DIR}/thirdparty/implot)
    #list(APPEND IMPLOT_SOURCES
    #  ${PROJECT_SOURCE_DIR}/thirdparty/implot/implot.h
    #  ${PROJECT_SOURCE_DIR}/thirdparty/implot/implot.cpp
    #  ${PROJECT_SOURCE_DIR}/thirdparty/implot/implot_internal.h
    #  ${PROJECT_SOURCE_DIR}/thirdparty/implot/implot_items.cpp
    #  )
    #add_library(implot ${IMPLOT_SOURCES})
    #list(APPEND LIBRARIES implot)
  endif()

  ## Source code
  include_directories("${PROJECT_SOURCE_DIR}")
  add_subdirectory(cannon)

  # Find unit tests
  file( GLOB_RECURSE TEST_SOURCES cannon/*.test.cpp )
  add_executable(unit_tests ${TEST_SOURCES})
  target_link_libraries(unit_tests Catch2::Catch2 cannon)
  catch_discover_tests(unit_tests)

  # Find scripts
  if (CANNON_BUILD_SCRIPTS)
    file( GLOB_RECURSE SCRIPT_SOURCES scripts/*.cpp )
    foreach (script_source ${SCRIPT_SOURCES})
      get_filename_component( scriptname ${script_source} NAME_WE)
      add_executable( ${scriptname} ${script_source})
      target_link_libraries( ${scriptname} cannon)
      target_include_directories( ${scriptname} PUBLIC "${PROJECT_SOURCE_DIR}")
      set_target_properties(${scriptname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/scripts")
    endforeach (script_source ${SCRIPT_SOURCES})
  endif()

  if (CANNON_BUILD_RESEARCH)
    # Find experiments
    file( GLOB_RECURSE EXP_SOURCES cannon/research/experiments/*.cpp )
    foreach (exp_source ${EXP_SOURCES})
      get_filename_component( expname ${exp_source} NAME_WE)
      add_executable( ${expname} ${exp_source} )
      target_link_libraries( ${expname} cannon)
      target_include_directories( ${expname} PUBLIC "${PROJECT_SOURCE_DIR}")
      set_target_properties(${expname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/experiments")
    endforeach (exp_source ${EXP_SOURCES})
  endif()
endif()
