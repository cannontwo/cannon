#include <iostream>
#include <fstream>
#include <string>

#include <cannon/logic/cnf.hpp>
#include <cannon/logic/write_dimacs_cnf.hpp>

using namespace cannon::logic;

int main() {
  unsigned int num_vars = 150;
  double clause_ratio = 3.0;

  while (clause_ratio < 6.2) {
    for (int i = 0; i < 100; i++) {
      unsigned int l = (unsigned int)std::floor(clause_ratio * num_vars);
      std::string filename = "formulas/random_3sat/N" + std::to_string(num_vars) +
        "L" + std::to_string(l) + "_" + std::to_string(i) + ".cnf";
      std::ofstream os(filename);

      os << write_cnf(generate_random_formula(num_vars, l));
      os.close();
    }

    clause_ratio += 0.2;
  }
}
