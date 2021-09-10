list(APPEND PLOT_SOURCES 
  line.cpp
  plotter.cpp
  scatter.cpp
  axes.cpp
  polygon.cpp
  )

add_library(plot OBJECT ${PLOT_SOURCES})
target_compile_options(plot PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
