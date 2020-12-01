#include <cannon/logic/cnf.hpp>

using namespace cannon::logic;

// Literal
PropAssignment Literal::eval(const Assignment& assignment) const {
  if (assignment.size() < (prop_ + 1)) {
    throw std::runtime_error("Assignment passed to literal has too few entries");
  }

  auto a = assignment[prop_];
  if (a == PropAssignment::Unassigned) 
    return PropAssignment::Unassigned;
  else { 
    if (negated_) 
      return a == PropAssignment::True ? PropAssignment::False : PropAssignment::True;
    else
      return a;
  }
}


// Clause
void Clause::add_literal(Literal l) {
  num_props_ = std::max(num_props_, l.prop_ + 1);
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

bool Clause::is_unit(const Assignment& a) const {
  int num_unassigned = 0;
  for (auto& l : literals_) {
    if (a[l.prop_] == PropAssignment::Unassigned)
      num_unassigned += 1;
  }

  return num_unassigned == 1;
}

std::set<unsigned int> Clause::get_props(const Assignment& a) {
  std::set<unsigned int> ret_set;
  for (auto& l : literals_) {
    if (a[l.prop_] == PropAssignment::Unassigned) {
      ret_set.insert(l.prop_);
    }
  }

  return ret_set;
}

PropAssignment Clause::eval(const Assignment& assignment) const {
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
  num_props_ = std::max(num_props_, c.num_props_);
  clauses_.push_back(c);
}

PropAssignment CNFFormula::eval(const Assignment& assignment,
    const Simplification& s) const {
  bool found_unassigned = false;

  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;

    auto a = clauses_[i].eval(assignment);
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

std::vector<std::pair<unsigned int, bool>> CNFFormula::get_unit_clause_props(const
    Assignment& a, const Simplification& s) const {
  std::vector<std::pair<unsigned int, bool>> idxs; 

  for (unsigned int i=0; i < clauses_.size(); i++) {
    // We only want to get unit clauses that have not yet been assigned or simplified
    if (s[i])
      continue;

    const Clause& c = clauses_[i];
    if (c.is_unit(a)) {
      for (auto& l : c.literals_) {
        if (l.eval(a) == PropAssignment::Unassigned) {
          idxs.emplace_back(l.prop_, l.negated_);
          break;
        }
      }
    }
  }

  return idxs;
}

unsigned int CNFFormula::get_num_props() const {
  return num_props_;
}

unsigned int CNFFormula::get_num_clauses() const {
  return clauses_.size();
}

std::vector<unsigned int> CNFFormula::get_props(const Assignment& a, const Simplification& s) {
  std::set<unsigned int> all_props;
  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;
    
    auto c_props = clauses_[i].get_props(a);
    all_props.insert(c_props.begin(), c_props.end());
  }

  std::vector<unsigned int> ret_vec;
  ret_vec.reserve(all_props.size());
  ret_vec.insert(ret_vec.end(), all_props.begin(), all_props.end());

  return ret_vec;
}

Simplification CNFFormula::simplify(const Assignment& a, const Simplification& s) const {
  std::valarray<bool> new_s(false, get_num_clauses());
    
  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i]) {
      new_s[i] = true;
      continue;  
    }

    if (clauses_[i].eval(a) == PropAssignment::True) {
      new_s[i] = true;
    }
  }

  return new_s;
}

// Free Functions
Clause cannon::logic::generate_random_clause(unsigned int num_props) {
  if (num_props < 3) {
    throw std::runtime_error("Cannot generate a random 3-SAT clause with fewer \
        than 3 propositions to choose from");
  }

  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_int_distribution<unsigned int> dist(0, num_props - 1);
  std::bernoulli_distribution n_dist(0.5);

  unsigned int p1 = dist(gen);
  unsigned int p2 = dist(gen);
  unsigned int p3 = dist(gen);

  // Whether to negate propositions
  bool n1 = n_dist(gen);
  bool n2 = n_dist(gen);
  bool n3 = n_dist(gen);

  // We need distinct propositions
  while (p2 == p1) {
    p2 = dist(gen);
  }
  while (p3 == p1 || p3 == p2) {
    p3 = dist(gen);
  }

  Clause c;
  c.add_literal(p1, n1);
  c.add_literal(p2, n2);
  c.add_literal(p3, n3);

  return c;
}

CNFFormula cannon::logic::generate_random_formula(unsigned int num_props, unsigned int num_clauses) {
  CNFFormula f;
  for (unsigned int i = 0; i < num_clauses; i++) {
    f.add_clause(generate_random_clause(num_props));
  }

  return f;
}

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

std::ostream& cannon::logic::operator<<(std::ostream& os, const std::valarray<PropAssignment>& v) {
  os << "[";
  if (v.size() > 0) {
    for (unsigned int i = 0; i < (v.size()-1); i++) {
      os << v[i] << ", ";
    }
  }
  os << v[v.size()-1] << "]";

  return os;
}

bool cannon::logic::operator==(const std::valarray<PropAssignment>& v1, const
    std::valarray<PropAssignment>& v2) {
  if (v1.size() != v2.size())
    return false;

  for (int i = 0; i < v1.size(); i++) {
    if (v1[i] != v2[i])
      return false;
  }

  return true;
}


bool cannon::logic::operator!=(const std::valarray<PropAssignment>& v1, const
    std::valarray<PropAssignment>& v2) {
  return !(v1 == v2);
}
