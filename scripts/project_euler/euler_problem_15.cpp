#include <iostream>
#include <utility>
#include <map>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * Starting in the top left corner of a 2x2 grid, and only being able to move
 * to the right and down, there are exactly 6 routes to the bottom right
 * corner.
 *
 * How many such routes are there in a 20x20 grid?
 */

unsigned int get_num_grid_routes_naive(unsigned int grid_rows, 
                                       unsigned int grid_cols, 
                                       unsigned int i, unsigned int j) {

  log_info("Getting paths for (", i, ",", j, ")");

  if (i == grid_rows || j == grid_cols)
    return 1;

  return get_num_grid_routes_naive(grid_rows, grid_cols, i+1, j) +
         get_num_grid_routes_naive(grid_rows, grid_cols, i, j+1);
}

unsigned long get_num_grid_routes(unsigned int grid_rows, 
                                 unsigned int grid_cols) {

  std::map<std::pair<unsigned int, unsigned int>, unsigned long> paths;

  for (unsigned int i = 0; i <= grid_rows; i++) {
    paths[std::make_pair(i, grid_cols)] = 1;
  }

  for (unsigned int j = 0; j <= grid_cols; j++) {
    paths[std::make_pair(grid_rows, j)] = 1;
  }

  for (int i = grid_rows-1; i >= 0; i--) {
    for (int j = grid_cols-1; j >= 0; j--) {
      log_info("Getting paths for (", i, ",", j, ")");

      unsigned long tmp = paths[std::make_pair(i+1, j)] +
        paths[std::make_pair(i, j+1)];

      paths[std::make_pair(i, j)] = tmp;
    }
  }

  return paths[std::make_pair(0, 0)];
}

int main(int argc, char** argv) {
  std::cout << get_num_grid_routes(20, 20) << std::endl;
}
