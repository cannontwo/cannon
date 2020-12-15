#ifndef CANNON_LOGIC_READ_DIMACS_CNF_H
#define CANNON_LOGIC_READ_DIMACS_CNF_H 

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <cannon/logic/cnf.hpp>

namespace cannon {
  namespace logic {

    // Parses a DIMACS specification for a CNF formula
    CNFFormula parse_cnf(const std::string& s);
    CNFFormula load_cnf(const std::string& path);

    Clause parse_clause(std::string& s);
    Clause parse_clause(const std::string& s);

  } // namespace logic
} // namespace cannon
#endif /* ifndef CANNON_LOGIC_READ_DIMACS_CNF_H */
