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

unsigned int Clause::size(const Assignment& a) const {
  unsigned int count = 0;

  for (auto &l : literals_) {
    if (a[l.prop_] == PropAssignment::Unassigned)
      count++;
  }

  return count;
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

bool Clause::contains_prop(const Assignment& a, unsigned int prop) const {
  for (auto& l : literals_) {
    if (a[l.prop_] == PropAssignment::Unassigned && l.prop_ == prop)
      return true;
  }

  return false;
}

bool Clause::has_pos_literal(const Assignment& a, unsigned int prop) const {
  for (auto& l : literals_) {
    if ((a[l.prop_] == PropAssignment::Unassigned) && (l.prop_ == prop) && !l.negated_)
      return true;
  }

  return false;
}

PropAssignment Clause::get_assignment_for_literal(unsigned int prop) {
  for (auto& l : literals_) {
    if (l.prop_ == prop) {
      if (l.negated_) {
        return PropAssignment::False;
      } else {
        return PropAssignment::True;
      }
    }
  }

  return PropAssignment::Unassigned;
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

unsigned int Clause::get_unit_prop(const Assignment& a) {
  assert(is_unit(a));

  for (auto &l : literals_) { 
    if (l.eval(a) == PropAssignment::Unassigned) {
      return l.prop_;
    }
  }

  throw std::runtime_error("Reached impossible point");
}

bool Clause::get_unit_negated(const Assignment& a) {
  assert(is_unit(a));

  for (auto &l : literals_) { 
    if (l.eval(a) == PropAssignment::Unassigned) {
      return l.negated_;
    }
  }

  throw std::runtime_error("Reached impossible point");
}

// CNFFormula 
void CNFFormula::add_clause(Clause&& c) {
  num_props_ = std::max(num_props_, c.num_props_);

  //for (Clause &c2 : clauses_) {
  //  if (c == c2) {
  //    return;
  //  }
  //}

  clauses_.emplace_back(c);
}

PropAssignment CNFFormula::eval(const Assignment& assignment,
    const Simplification& s) const {
  bool found_unassigned = false;

  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;

    auto a = clauses_[i].eval(assignment);
    if (a == PropAssignment::False) {
      return PropAssignment::False;
    } else if (a == PropAssignment::Unassigned)
      found_unassigned = true;
  }

  if (found_unassigned)
    return PropAssignment::Unassigned;
  else
    return PropAssignment::True;
}

std::vector<std::tuple<unsigned int, bool, int>> CNFFormula::get_unit_clause_props(const
    Assignment& a, const Simplification& s) const {
  std::vector<std::tuple<unsigned int, bool, int>> idxs; 

  for (unsigned int i=0; i < clauses_.size(); i++) {
    // We only want to get unit clauses that have not yet been assigned or simplified
    if (s[i])
      continue;

    const Clause& c = clauses_[i];
    if (c.is_unit(a)) {
      for (auto& l : c.literals_) {
        if (l.eval(a) == PropAssignment::Unassigned) {
          idxs.emplace_back(l.prop_, l.negated_, i);
          break;
        }
      }
    }
  }

  return idxs;
}

std::vector<std::tuple<unsigned int, bool, int>> CNFFormula::get_unit_clause_props(const
    Assignment& a, const Simplification& s, std::vector<unsigned int> unit_clauses) const {
  std::vector<std::tuple<unsigned int, bool, int>> idxs; 

  for (unsigned int c_num : unit_clauses) {
    if (clauses_[c_num].is_unit(a)) {
      for (auto& l : clauses_[c_num].literals_) {
        if (l.eval(a) == PropAssignment::Unassigned) {
          idxs.emplace_back(l.prop_, l.negated_, c_num);
          break;
        }
      }
    }
  }

  return idxs;
}

std::vector<std::tuple<unsigned int, bool, int>> CNFFormula::get_unit_clause_props(const
    Assignment& a, const Simplification& s,
    std::vector<std::vector<unsigned int>> watched) const {
  std::vector<std::tuple<unsigned int, bool, int>> idxs; 

  for (unsigned int i=0; i < get_num_props(); i++) {
    // We only want to get unit clauses that have not yet been assigned or simplified
    for (unsigned int c_num : watched[i]) {
      if (s[c_num])
        continue;

      if (clauses_[c_num].is_unit(a)) {
        for (auto& l : clauses_[c_num].literals_) {
          if (l.eval(a) == PropAssignment::Unassigned) {
            idxs.emplace_back(l.prop_, l.negated_, c_num);
            break;
          }
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

unsigned int CNFFormula::get_num_two_clauses(const Assignment& a, const
    Simplification& s, unsigned int prop) const {
  int num_two_clauses = 0; 
  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;

    if (clauses_[i].size(a) == 2 && clauses_[i].contains_prop(a, prop))
      num_two_clauses++;
  }

  return num_two_clauses;
}

std::vector<unsigned int> CNFFormula::get_num_two_clauses(const Assignment& a, const
    Simplification& s, const std::vector<unsigned int>& props) const {
  std::vector<unsigned int> num_two_clauses(props.size(), 0); 

  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;

    if (clauses_[i].size(a) == 2) {
      for (auto &l : clauses_[i].literals_) {
        if (a[l.prop_] != PropAssignment::Unassigned)
          continue;

        auto it = std::find(props.begin(), props.end(), l.prop_);
        if (it != props.end()) {
          num_two_clauses[std::distance(props.begin(), it)] += 1;  
        }
      }
    }
  }

  return num_two_clauses;
}


unsigned int CNFFormula::get_smallest_clause_size(const Assignment& a, const Simplification& s) const {
  if (clauses_.size() == 0)
    return 0;

  unsigned int smallest_size = clauses_[0].size();
  for (unsigned int i = 0; i < clauses_.size(); i++) {
    if (s[i])
      continue;

    if (clauses_[i].size(a) < smallest_size)
      smallest_size = clauses_[i].size(a);
  }

  return smallest_size;
}

std::vector<unsigned int> CNFFormula::get_props(const Assignment& a, const Simplification& s) {
  // Way too inefficient
  //std::set<unsigned int> all_props;
  //for (unsigned int i = 0; i < get_num_clauses(); i++) {
  //  if (s[i])
  //    continue;
  //  
  //  auto c_props = clauses_[i].get_props(a);
  //  all_props.insert(c_props.begin(), c_props.end());
  //}
  //ret_vec.reserve(all_props.size());
  //ret_vec.insert(ret_vec.end(), all_props.begin(), all_props.end());

  std::vector<unsigned int> ret_vec;
  for (unsigned int i = 0; i < a.size(); i++) {
    if (a[i] == PropAssignment::Unassigned) {
      ret_vec.push_back(i);
    }
  }

  return ret_vec;
}

MatrixXd CNFFormula::make_adjacency_mat(const Assignment& a, const Simplification& s) const {
  MatrixXd adj_mat(get_num_props(), get_num_props());

  for (unsigned int i = 0; i < clauses_.size(); i++) {
    if (s[i])
      continue;

    std::vector<unsigned int> ps;
    for (auto &l : clauses_[i].literals_) {
      if (a[l.prop_] == PropAssignment::Unassigned) {
        ps.push_back(l.prop_);  
      }
    }

    for (unsigned int i = 0; i < ps.size(); i++) {
      for (unsigned int j = i+1; j < ps.size(); j++) {
        adj_mat(ps[i], ps[j]) = 1.0;
        adj_mat(ps[j], ps[i]) = 1.0;
      }
    }
  }

  return adj_mat;
}

std::multiset<unsigned int> CNFFormula::get_props_multiset(const Assignment& a, const
    Simplification& s) {
  std::multiset<unsigned int> all_props;
  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;
    
    auto c_props = clauses_[i].get_props(a);
    all_props.insert(c_props.begin(), c_props.end());
  }

  return all_props;
}

PropAssignment CNFFormula::is_pure_literal(const Assignment& a, const Simplification& s,
    unsigned int prop) {
  PropAssignment tmp = PropAssignment::Unassigned;

  for (unsigned int i = 0; i < get_num_clauses(); i++) {
    if (s[i])
      continue;

    if (tmp == PropAssignment::Unassigned) {
      // This is expensive, but should only be called during preprocessing
      tmp = clauses_[i].get_assignment_for_literal(prop);
    } else {
      PropAssignment t2 = clauses_[i].get_assignment_for_literal(prop);
      if (t2 != PropAssignment::Unassigned && tmp != t2) {
        return PropAssignment::Unassigned;
      }
    }
  }

  return tmp;
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

void cannon::logic::resolve(Clause& c1, const Clause& c2, unsigned int prop) {
  bool c1_has_prop = false;
  bool c1_negated = false;
  bool c2_has_prop = false;
  bool c2_negated = false;
  Clause ret_clause;
  std::set<Literal>::iterator rem_it = c1.literals_.end();

  for (auto it = c1.literals_.begin(); it != c1.literals_.end(); it++) {
    const Literal& l = *it;
    if (l.prop_ == prop) {
      c1_has_prop = true;
      c1_negated = l.negated_;
      rem_it = it;
      continue;
    }
  }

  assert(rem_it != c1.literals_.end());
  c1.literals_.erase(rem_it);

  for (auto& l : c2.literals_) {
    if (l.prop_ == prop) {
      c2_has_prop = true;
      c2_negated = l.negated_;
      continue;
    }

    c1.add_literal(l.prop_, l.negated_);
  }

  if ((!c1_has_prop || !c2_has_prop) || (c1_negated == c2_negated))
    throw std::runtime_error("Applied resolution to incompatible clauses");
}

std::ostream& cannon::logic::operator<<(std::ostream& os, PropAssignment a) {
  switch (a) {
    case PropAssignment::True: os << "True"; break;
    case PropAssignment::False: os << "False"; break;
    case PropAssignment::Unassigned: os << "Unassigned"; break;
  }

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
  if (v.size() > 0) {
    os << "[" << std::endl;
    for (unsigned int i = 0; i < (v.size()-1); i++) {
      os << "p" << i << " = " << v[i] << ", " << std::endl;
    }
    os << "p" << v.size() - 1 << " = " << v[v.size()-1] << "]";
  } else {
    os << "[]";
  }

  return os;
}

bool cannon::logic::operator==(const std::valarray<PropAssignment>& v1, const
    std::valarray<PropAssignment>& v2) {
  if (v1.size() != v2.size())
    return false;

  for (unsigned int i = 0; i < v1.size(); i++) {
    if (v1[i] != v2[i])
      return false;
  }

  return true;
}


bool cannon::logic::operator!=(const std::valarray<PropAssignment>& v1, const
    std::valarray<PropAssignment>& v2) {
  return !(v1 == v2);
}
