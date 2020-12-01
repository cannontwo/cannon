#ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_
#define CANNON_LOGIC_WRITE_DIMACS_CNF_H_ 

#include <string>
#include <sstream>

#include <cannon/logic/cnf.hpp>

namespace cannon {
  namespace logic {

    std::string write_cnf(const CNFFormula& f);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_ */
