#include <iostream>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

/*!
 * By starting at the top of the triangle below and moving to adjacent numbers
 * on the row below, the maximum total from top to bottom is 23. 
 *
 *                         3
 *                        7 4
 *                       2 4 6
 *                      8 5 9 3
 *
 * That is, 3 + 7 + 4 + 9 = 23. 
 *
 * Find the maximum total from top to bottom of the triangle below.
 */

static const unsigned int TRIANGLE[] = {
  75,
  95, 64,
  17, 47, 82,
  18, 35, 87, 10,
  20, 4, 82, 47, 65,
  19, 1, 23, 75, 3, 34,
  88, 2, 77, 73, 7, 63, 67,
  99, 65, 4, 28, 6, 16, 70, 92,
  41, 41, 26, 56, 83, 40, 80, 70, 33,
  41, 48, 72, 33, 47, 32, 37, 16, 94, 29,
  53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14,
  70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57,
  91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48,
  63, 66, 4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31,
  4, 62, 98, 27, 23, 9, 70, 98, 73, 93, 38, 53, 60, 4, 23
};

std::pair<unsigned int, unsigned int> get_triangle_row_min_max(unsigned int row) {
  assert(row < 15);

  // Sum of row lengths before this one
  unsigned int row_start = row * (row + 1) / 2;

  // Row has length = row + 1
  return std::make_pair(row_start, row_start + row);
}

unsigned int get_triangle_max_path () {
  std::vector<unsigned int> next_row_max_paths(16, 0);

  for (int row = 14; row >= 0; --row) {
    unsigned int row_start, row_end;
    std::tie(row_start, row_end) = get_triangle_row_min_max(row);

    // At each row, we choose the child with the greatest path
    std::vector<unsigned int> row_max_paths;
    for (unsigned int i = 0; i <= row_end - row_start; i++) {
      unsigned int tri_val = TRIANGLE[row_start + i];
      unsigned int left = next_row_max_paths[i];
      unsigned int right = next_row_max_paths[i+1];
      row_max_paths.push_back(tri_val + std::max(left, right));
    }

    next_row_max_paths = row_max_paths;
  }

  return next_row_max_paths[0];
}

int main(int argc, char** argv) {
  std::cout << get_triangle_max_path() << std::endl;
}
