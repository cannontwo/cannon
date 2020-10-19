#include <cannon/logic/dpll.hpp>

using namespace cannon::logic;

std::ostream& cannon::logic::operator<<(std::ostream& os, const DPLLResult& r) {
  if (r == DPLLResult::Satisfiable)
    os << "Satisfiable";
  else if (r == DPLLResult::Unknown)
    os << "Unknown";
  else
    os << "Unsatisfiable";

  return os;
}

