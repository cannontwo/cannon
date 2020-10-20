#include <sstream>
#include <cassert>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

int main() {
  // PropAssignment printing
  std::stringstream ss;
  ss << PropAssignment::True;
  assert(ss.str().compare("True") == 0);
  ss.str("");

  ss << PropAssignment::False;
  assert(ss.str().compare("False") == 0);
  ss.str("");
  
  ss << PropAssignment::Unassigned;
  assert(ss.str().compare("Unassigned") == 0);
  ss.str("");

  // Test literal evaluation
  std::valarray<PropAssignment> a1 = {PropAssignment::True, PropAssignment::False};
  std::valarray<PropAssignment> a2 = {PropAssignment::False, PropAssignment::True};
  std::valarray<PropAssignment> a3 = {PropAssignment::Unassigned, PropAssignment::Unassigned};
  std::valarray<PropAssignment> a4 = {PropAssignment::Unassigned, PropAssignment::False};
  std::valarray<PropAssignment> a5 = {PropAssignment::True, PropAssignment::Unassigned};
  std::valarray<PropAssignment> a6 = {PropAssignment::Unassigned, PropAssignment::True};

  Literal l_fail(10, false);
  try {
    l_fail.eval({PropAssignment::True});
    assert(false);
  } catch (...) {}
  

  Literal l1(0, false);
  Literal l2(1, true);

  assert(l1.eval(a1) == PropAssignment::True);
  assert(l2.eval(a1) == PropAssignment::True);
  assert(l1.eval(a2) == PropAssignment::False);
  assert(l2.eval(a2) == PropAssignment::False);
  assert(l1.eval(a3) == PropAssignment::Unassigned);
  assert(l2.eval(a3) == PropAssignment::Unassigned);

  // Testing clause construction and evaluation
  Clause c;

  c.add_literal(0, false);
  assert(c.size() == 1);
  assert(c.is_unit());
  assert(!c.is_unit(a1));
  assert(c.is_unit(a3));
  assert(c.get_props(a3) == std::set<unsigned int>({0}));
  assert(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(0, true);
  assert(c.size() == 2);
  assert(!c.is_unit());
  assert(c.get_props(a3) == std::set<unsigned int>({0}));
  assert(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(1, false);
  assert(c.size() == 3);
  assert(!c.is_unit());
  assert(c.is_unit(a5));
  assert(c.get_props(a3) == std::set<unsigned int>({0, 1}));
  assert(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(0, false);
  assert(c.size() == 3);
  assert(!c.is_unit());

  assert(c.eval(a1) == PropAssignment::True);
  assert(c.eval(a2) == PropAssignment::True);
  assert(c.eval(a3) == PropAssignment::Unassigned);
  assert(c.eval(a4) == PropAssignment::Unassigned);

  ss << c;
  log_info(ss.str());
  assert(ss.str().compare("( !p0 v p0 v p1 )") == 0);

  // Testing formula construction
  CNFFormula f;
  Clause c2;

  c2.add_literal(0, false);
  c2.add_literal(0, false);
  f.add_clause(c);
  f.add_clause(c2);
  log_info(f);

  // Fake simplification array
  std::valarray<bool> s(false, 2);

  assert(f.eval(a1, s) == PropAssignment::True);
  assert(f.eval(a2, s) == PropAssignment::False);
  assert(f.eval(a3, s) == PropAssignment::Unassigned);
  assert(f.eval(a4, s) == PropAssignment::Unassigned);
  assert(f.eval(a5, s) == PropAssignment::True);

  assert(f.get_props(a3, s) == std::vector<unsigned int>({0, 1}));
  assert(f.get_props(a1, s) == std::vector<unsigned int>({}));

  std::valarray<bool> new_s(false, 2);
  new_s[0] = true;
  assert(f.simplify(a6, s)[0] == new_s[0] && f.simplify(a6, s)[1] == new_s[1]);

  // Testing getting unit clauses
  assert(f.get_unit_clause_props(a1, s).size() == 0);
  auto p = std::pair<unsigned int, bool>(0, false);
  assert(f.get_unit_clause_props(a3, s)[0] == p);
}
