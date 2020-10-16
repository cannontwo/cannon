#include <sstream>
#include <cassert>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

int main() {
  // PropAssignment printing
  std::stringstream ss;
  ss << True;
  assert(ss.str().compare("True") == 0);
  ss.str("");

  ss << False;
  assert(ss.str().compare("False") == 0);
  ss.str("");
  
  ss << Unassigned;
  assert(ss.str().compare("Unassigned") == 0);
  ss.str("");

  // Test literal evaluation
  std::valarray<PropAssignment> a1 = {True, False};
  std::valarray<PropAssignment> a2 = {False, True};
  std::valarray<PropAssignment> a3 = {Unassigned, Unassigned};
  std::valarray<PropAssignment> a4 = {Unassigned, False};
  std::valarray<PropAssignment> a5 = {True, Unassigned};

  Literal l_fail(10, false);
  try {
    l_fail.eval({True});
    assert(false);
  } catch (...) {}
  

  Literal l1(0, false);
  Literal l2(1, true);

  assert(l1.eval(a1) == True);
  assert(l2.eval(a1) == True);
  assert(l1.eval(a2) == False);
  assert(l2.eval(a2) == False);
  assert(l1.eval(a3) == Unassigned);
  assert(l2.eval(a3) == Unassigned);

  // Testing clause construction and evaluation
  Clause c;

  c.add_literal(0, false);
  assert(c.size() == 1);
  assert(c.is_unit());
  assert(!c.is_unit(a1));
  assert(c.is_unit(a3));

  c.add_literal(0, true);
  assert(c.size() == 2);
  assert(!c.is_unit());

  c.add_literal(1, false);
  assert(c.size() == 3);
  assert(!c.is_unit());
  assert(c.is_unit(a5));

  c.add_literal(0, false);
  assert(c.size() == 3);
  assert(!c.is_unit());

  assert(c.eval(a1) == True);
  assert(c.eval(a2) == True);
  assert(c.eval(a3) == Unassigned);
  assert(c.eval(a4) == Unassigned);

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

  assert(f.eval(a1) == True);
  assert(f.eval(a2) == False);
  assert(f.eval(a3) == Unassigned);
  assert(f.eval(a4) == Unassigned);
  assert(f.eval(a5) == True);

  // Testing getting unit clauses
  assert(f.get_unit_clauses(a1).size() == 0);
  auto p = std::pair<unsigned int, bool>(0, false);
  assert(f.get_unit_clauses(a3)[0] == p);
}
