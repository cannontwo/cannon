#include <cannon/logic/write_dimacs_cnf.hpp>

using namespace cannon::logic;

std::string cannon::logic::write_cnf(const CNFFormula& f) {
  std::stringstream ss;

  ss << "p cnf ";
  ss << std::to_string(f.get_num_props());
  ss << " ";
  ss << std::to_string(f.get_num_clauses()) << std::endl;

  for (auto &c : f.clauses_) {
    for (auto &l : c.literals_) {
      // Internally using 0-indexing, so we have to add 1
      int prop = l.prop_ + 1;
      if (l.negated_) {
        prop = -prop;
      }
      ss << std::to_string(prop) << " ";
    } 
    ss << "0" << std::endl;
  }

  return ss.str();
}
