#include <cannon/logic/cnf.hpp>

using namespace cannon::logic;

// Literal
PropAssignment Literal::eval(const std::valarray<PropAssignment> assignment) const {
  if (assignment.size() < prop_ + 1) {
    throw std::runtime_error("Assignment passed to literal has too few entries");
  }

  auto a = assignment[prop_];
  if (a == PropAssignment::Unassigned) 
    return PropAssignment::Unassigned;
  else { 
    if (a == PropAssignment::True) 
      return negated_ ? PropAssignment::False : PropAssignment::True;
    else
      return negated_ ? PropAssignment::True : PropAssignment::False;
  }
}


// Clause
void Clause::add_literal(Literal l) {
  literals_.insert(l);
}

void Clause::add_literal(unsigned int prop_num, bool negated) {
  add_literal({prop_num, negated});
}

unsigned int Clause::size() const {
  return literals_.size();
}

bool Clause::is_unit() const {
  return literals_.size() == 1;
}

PropAssignment Clause::eval(const std::valarray<PropAssignment> assignment) const {
  bool found_unassigned = false;

  for (auto& literal : literals_) {
    auto a = literal.eval(assignment);
    if (a == PropAssignment::True) 
      return PropAssignment::True;
    else if (a == PropAssignment::Unassigned) {
      found_unassigned = true;
    }
  }

  // If we found at least one unassigned literal, this clause could still
  // evaluate to true.
  if (found_unassigned)
    return PropAssignment::Unassigned;
  else
    return PropAssignment::False;
}

// CNFFormula 
void CNFFormula::add_clause(Clause c) {
  clauses_.push_back(c);
}

PropAssignment CNFFormula::eval(const std::valarray<PropAssignment> assignment) const {
  bool found_unassigned = false;
  for (auto& clause : clauses_) {
    auto a = clause.eval(assignment);
    if (a == PropAssignment::False)
      return PropAssignment::False;
    else if (a == PropAssignment::Unassigned)
      found_unassigned = true;
  }

  if (found_unassigned)
    return PropAssignment::Unassigned;
  else
    return PropAssignment::True;
}

std::vector<std::pair<unsigned int, bool>> CNFFormula::get_unit_clauses(const
    std::valarray<PropAssignment>& a) const {
  std::vector<std::pair<unsigned int, bool>> idxs; 

  for (auto& c : clauses_) {
    // We only want to get unit clauses that have not yet been assigned
    if (c.is_unit() && c.eval(a) == PropAssignment::Unassigned) {
      const Literal& l = *c.literals_.begin();
      idxs.emplace_back(l.prop_, l.negated_);
    }
  }

  return idxs;
}

// Free Functions
std::ostream& cannon::logic::operator<<(std::ostream& os, const PropAssignment& a) {
  if (a == PropAssignment::True)
    os << "True";
  else if (a == PropAssignment::False)
    os << "False";
  else
    os << "Unassigned";

  return os;
}

std::ostream& cannon::logic::operator<<(std::ostream& os, const Literal& l) {
  if (l.negated_) {
    os << "!";
  }
  os << "p" << l.prop_;

  return os;
}

std::ostream& cannon::logic::operator<<(std::ostream& os, const Clause& c) {
  os << "( ";
  for (auto it = c.literals_.begin(); it != c.literals_.end(); ++it) {
    os << *it;
    if (std::next(it) != c.literals_.end()) {
      os << " v ";
    }
  }

  os << " )";

  return os;
}

std::ostream& cannon::logic::operator<<(std::ostream& os, const CNFFormula& f) {
  os << "{";
  for (auto& c : f.clauses_) {
    os << "\n" << c << ","; 
  }

  os << "\n}";
  return os;
}
