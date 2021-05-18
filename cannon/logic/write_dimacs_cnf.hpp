#ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_
#define CANNON_LOGIC_WRITE_DIMACS_CNF_H_ 

#include <string>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);

    std::string write_cnf(const CNFFormula& f);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_ */
