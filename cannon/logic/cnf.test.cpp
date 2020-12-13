#include <catch2/catch.hpp>

#include <sstream>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

TEST_CASE("CNF", "[logic]") {
  // PropAssignment printing
  std::stringstream ss;
  ss << PropAssignment::True;
  REQUIRE(ss.str().compare("True") == 0);
  ss.str("");

  ss << PropAssignment::False;
  REQUIRE(ss.str().compare("False") == 0);
  ss.str("");
  
  ss << PropAssignment::Unassigned;
  REQUIRE(ss.str().compare("Unassigned") == 0);
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
    REQUIRE(false);
  } catch (...) {}
  

  Literal l1(0, false);
  Literal l2(1, true);

  REQUIRE(l1.eval(a1) == PropAssignment::True);
  REQUIRE(l2.eval(a1) == PropAssignment::True);
  REQUIRE(l1.eval(a2) == PropAssignment::False);
  REQUIRE(l2.eval(a2) == PropAssignment::False);
  REQUIRE(l1.eval(a3) == PropAssignment::Unassigned);
  REQUIRE(l2.eval(a3) == PropAssignment::Unassigned);

  // Testing clause construction and evaluation
  Clause c;

  c.add_literal(0, false);
  REQUIRE(c.size() == 1);
  REQUIRE(c.is_unit());
  REQUIRE(!c.is_unit(a1));
  REQUIRE(c.is_unit(a3));
  REQUIRE(c.get_props(a3) == std::set<unsigned int>({0}));
  REQUIRE(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(0, true);
  REQUIRE(c.size() == 2);
  REQUIRE(!c.is_unit());
  REQUIRE(c.get_props(a3) == std::set<unsigned int>({0}));
  REQUIRE(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(1, false);
  REQUIRE(c.size() == 3);
  REQUIRE(!c.is_unit());
  REQUIRE(c.is_unit(a5));
  REQUIRE(c.get_props(a3) == std::set<unsigned int>({0, 1}));
  REQUIRE(c.get_props(a1) == std::set<unsigned int>({}));

  c.add_literal(0, false);
  REQUIRE(c.size() == 3);
  REQUIRE(!c.is_unit());

  REQUIRE(c.eval(a1) == PropAssignment::True);
  REQUIRE(c.eval(a2) == PropAssignment::True);
  REQUIRE(c.eval(a3) == PropAssignment::Unassigned);
  REQUIRE(c.eval(a4) == PropAssignment::Unassigned);

  ss << c;
  REQUIRE(ss.str().compare("( !p0 v p0 v p1 )") == 0);

  // Testing formula construction
  CNFFormula f;
  Clause c2;

  c2.add_literal(0, false);
  c2.add_literal(0, false);
  f.add_clause(std::move(c));
  f.add_clause(std::move(c2));

  // Fake simplification array
  std::valarray<bool> s(false, 2);

  REQUIRE(f.eval(a1, s) == PropAssignment::True);
  REQUIRE(f.eval(a2, s) == PropAssignment::False);
  REQUIRE(f.eval(a3, s) == PropAssignment::Unassigned);
  REQUIRE(f.eval(a4, s) == PropAssignment::Unassigned);
  REQUIRE(f.eval(a5, s) == PropAssignment::True);

  REQUIRE(f.get_props(a3, s) == std::vector<unsigned int>({0, 1}));
  REQUIRE(f.get_props(a1, s) == std::vector<unsigned int>({}));

  std::valarray<bool> new_s(false, 2);
  new_s[0] = true;
  REQUIRE((f.simplify(a6, s)[0] == new_s[0] && f.simplify(a6, s)[1] == new_s[1]));

  // Testing getting unit clauses
  REQUIRE(f.get_unit_clause_props(a1, s).size() == 0);
  REQUIRE(std::get<0>(f.get_unit_clause_props(a3, s)[0]) == 0);
}
