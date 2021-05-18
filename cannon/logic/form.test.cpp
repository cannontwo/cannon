#include <catch2/catch.hpp>

#include <sstream>
#include <vector>
#include <string>

#include <cannon/logic/form.hpp>
#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;
using namespace cannon::logic;

TEST_CASE("Form", "[logic]") {
  // Basic printing
  // Basic eval
  auto f1 = Atomic(0);
  std::vector<bool> t = {true};
  REQUIRE(f1.eval(t));
  t[0] = false;
  REQUIRE(!f1.eval(t));

  auto f2 = make_negation(std::make_shared<Atomic>(0));
  t[0] = false;
  REQUIRE(f2->eval(t));
  t[0] = true;
  REQUIRE(!f2->eval(t));

  auto f3 = make_and(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1));
  t[0] = true;
  t.push_back(true);
  REQUIRE(f3->eval(t));
  t[0] = false;
  REQUIRE(!f3->eval(t));
  t[1] = false;
  REQUIRE(!f3->eval(t));
  t[0] = true;
  REQUIRE(!f3->eval(t));

  auto f4 = make_or(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1));
  t[0] = true;
  t[1] = true;
  REQUIRE(f4->eval(t));
  t[0] = false;
  REQUIRE(f4->eval(t));
  t[1] = false;
  REQUIRE(!f4->eval(t));
  t[0] = true;
  REQUIRE(f4->eval(t));

  auto f5 = make_implies(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1));
  t[0] = true;
  t[1] = true;
  REQUIRE(f5->eval(t));
  t[0] = false;
  REQUIRE(f5->eval(t));
  t[1] = false;
  REQUIRE(f5->eval(t));
  t[0] = true;
  REQUIRE(!f5->eval(t));

  auto f6 = make_iff(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1));
  t[0] = true;
  t[1] = true;
  REQUIRE(f6->eval(t));
  t[0] = false;
  REQUIRE(!f6->eval(t));
  t[1] = false;
  REQUIRE(f6->eval(t));
  t[0] = true;
  REQUIRE(!f6->eval(t));

  // Nested  
  auto alpha = make_and(make_implies(std::make_shared<Atomic>(0),
        make_and(std::make_shared<Atomic>(1), std::make_shared<Atomic>(2))),
      make_implies(make_negation(std::make_shared<Atomic>(0)),
        make_and(std::make_shared<Atomic>(2), std::make_shared<Atomic>(3))));

  // Throwing if not enough values in truth assignment
  try {
    alpha->eval(t);
    REQUIRE(false);
  } catch (...) {}
}

TEST_CASE("FormToCNF", "[logic]") {
  CNFFormula f1;
  Clause c1;
  c1.add_literal(0, false);
  f1.add_clause(std::move(c1));
  REQUIRE(Atomic(0).to_cnf() == f1);

  CNFFormula f2;
  Clause c2;
  c2.add_literal(0, true);
  f2.add_clause(std::move(c2));
  REQUIRE(make_negation(std::make_shared<Atomic>(0))->to_cnf() == f2);

  CNFFormula f3;
  Clause c3;
  Clause c4;
  c3.add_literal(0, false);
  c4.add_literal(1, false);
  f3.add_clause(std::move(c3));
  f3.add_clause(std::move(c4));
  REQUIRE(make_and(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1))->to_cnf() == f3);

  CNFFormula f4;
  Clause c5;
  c5.add_literal(0, false);
  c5.add_literal(1, false);
  f4.add_clause(std::move(c5));
  REQUIRE(make_or(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1))->to_cnf() == f4);

  CNFFormula f5;
  Clause c6;
  c6.add_literal(0, true);
  c6.add_literal(1, false);
  f5.add_clause(std::move(c6));
  REQUIRE(make_implies(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1))->to_cnf() == f5);

  CNFFormula f6;
  Clause c7;
  c7.add_literal(0, true);
  c7.add_literal(0, false);
  f6.add_clause(std::move(c7));

  Clause c8;
  c8.add_literal(0, false);
  c8.add_literal(1, true);
  f6.add_clause(std::move(c8));

  Clause c9;
  c9.add_literal(0, true);
  c9.add_literal(1, false);
  f6.add_clause(std::move(c9));

  Clause c10;
  c10.add_literal(1, true);
  c10.add_literal(1, false);
  f6.add_clause(std::move(c10));

  REQUIRE(make_iff(std::make_shared<Atomic>(0), std::make_shared<Atomic>(1))->to_cnf() == f6);

  CNFFormula f7;
  Clause c11;
  c11.add_literal(0, true);
  Clause c12;
  c12.add_literal(1, true);
  f7.add_clause(std::move(c11));
  f7.add_clause(std::move(c12));

  REQUIRE(make_negation(make_or(std::make_shared<Atomic>(0),
          std::make_shared<Atomic>(1)))->to_cnf() == f7);

  CNFFormula f8;
  Clause c13;
  c13.add_literal(0, true);
  c13.add_literal(1, true);
  f8.add_clause(std::move(c13));

  REQUIRE(make_negation(make_and(std::make_shared<Atomic>(0),
          std::make_shared<Atomic>(1)))->to_cnf() == f8);


}
