list(APPEND PLOT_SOURCES 
  plotter.cpp
  scatter.cpp
  axes.cpp
  )

add_library(plot ${PLOT_SOURCES})
target_link_libraries(plot graphics log ${LIBRARIES})
