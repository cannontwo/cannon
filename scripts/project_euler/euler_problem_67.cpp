#include <iostream>
#include <fstream>
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
 * Find the maximum total from top to bottom of the triangle in the input file.
 */

std::vector<unsigned int> load_triangle_file(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<unsigned int> triangle;

  unsigned int entry;
  while (file >> entry) {
    triangle.push_back(entry);
  }

  return triangle;
}

std::pair<unsigned int, unsigned int> get_triangle_row_min_max(unsigned int row) {
  assert(row < 15);

  // Sum of row lengths before this one
  unsigned int row_start = row * (row + 1) / 2;

  // Row has length = row + 1
  return std::make_pair(row_start, row_start + row);
}

unsigned int get_num_rows(const std::vector<unsigned int>& triangle) {
  unsigned int i = 0;
  while (i*(i+1) / 2 < triangle.size()) {
    i++;
  }

  assert(i*(i+1)/2 == triangle.size());

  return i;
}

unsigned int get_triangle_max_path (std::vector<unsigned int> triangle) {

  unsigned int num_rows = get_num_rows(triangle);
  log_info("Loaded triangle with", num_rows, "rows");

  std::vector<unsigned int> next_row_max_paths(num_rows + 1, 0);

  for (int row = num_rows-1; row >= 0; --row) {
    unsigned int row_start, row_end;
    std::tie(row_start, row_end) = get_triangle_row_min_max(row);

    // At each row, we choose the child with the greatest path
    std::vector<unsigned int> row_max_paths;
    for (unsigned int i = 0; i <= row_end - row_start; i++) {
      unsigned int tri_val = triangle[row_start + i];
      unsigned int left = next_row_max_paths[i];
      unsigned int right = next_row_max_paths[i+1];
      row_max_paths.push_back(tri_val + std::max(left, right));
    }

    next_row_max_paths = row_max_paths;
  }

  return next_row_max_paths[0];
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Please provide a file to read the triangle from" << std::endl;
    return 1;
  }

  auto triangle = load_triangle_file(argv[1]);

  std::cout << get_triangle_max_path(triangle) << std::endl;
}
