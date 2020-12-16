#include <catch2/catch.hpp>

#include <random>

#include <cannon/logic/dpll.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>

using namespace cannon::logic;

unsigned int uniform_random_prop(const DPLLState& ds,
    const Assignment& a, const Simplification& s, std::vector<unsigned int>& props) {
  RandomComparator comp(ds);
  std::sort(props.begin(), props.end(), comp);

  return props[props.size() - 1];
}

unsigned int vsids_prop(const DPLLState& ds, const Assignment& a, const Simplification& s,
    std::vector<unsigned int>& props) {
  RandomComparator rc(ds);
  VsidsComparator vc(ds, rc);
  TwoClauseComparator tc(ds, a, s, props, vc);

  std::sort(props.begin(), props.end(), tc);
  return props[props.size() - 1];
}

bool det_assign(const CNFFormula& form, const Assignment& a, 
    const Simplification& s, unsigned int prop, std::vector<std::vector<unsigned int>> watched) {
  return true;
}

bool uniform_random_assign(const CNFFormula& form, const Assignment& a, 
    const Simplification& s, unsigned int prop, std::vector<std::vector<unsigned int>> watched) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> d;

  double t = d(gen);
  return t < 0.5;
}

TEST_CASE("DPLL", "[logic]") {
  DPLLResult r;
  Assignment a;
  int c;

  // Empty
  CNFFormula f;
  std::tie(r, a, c) = dpll(f, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
  REQUIRE(r == DPLLResult::Satisfiable);

  // Basic
  CNFFormula f1;
  f1.add_clause(parse_clause("1"));

  std::tie(r, a, c) = dpll(f1, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
  REQUIRE(r == DPLLResult::Satisfiable);

  Assignment a1(1);
  a1[0] = PropAssignment::True;
  REQUIRE(a.size() == 1);
  //REQUIRE(a[0] == a1[0]);

  // True and False
  CNFFormula f2;
  f2.add_clause(parse_clause("1"));
  f2.add_clause(parse_clause("-2"));

  std::tie(r, a, c) = dpll(f2, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
  REQUIRE(r == DPLLResult::Satisfiable);

  Assignment a2(2); 
  a2[0] = PropAssignment::True; 
  a2[1] = PropAssignment::False;
  REQUIRE(a.size() == 2);
  REQUIRE((a[0] == a2[0] && a[1] == a2[1]));

  // Unsatisfiable
  CNFFormula f3;
  f3.add_clause(parse_clause("1"));
  f3.add_clause(parse_clause("-1"));

  std::tie(r, a, c) = dpll(f3, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
  REQUIRE(r == DPLLResult::Unsatisfiable);

  // Requiring splitting
  CNFFormula f4;
  f4.add_clause(parse_clause("-1 2"));
  f4.add_clause(parse_clause("-2 3"));
  f4.add_clause(parse_clause("-3 4"));
  f4.add_clause(parse_clause("-4 1"));
  f4.add_clause(parse_clause("1 2 3 4"));

  std::tie(r, a, c) = dpll(f4, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
  REQUIRE(r == DPLLResult::Satisfiable);

  // Backjumping bug testing
  CNFFormula f5 = load_cnf("formulas/test_3sat/N10L40_0.cnf");
  std::cout << "test_backjump formula is " << f5 << std::endl;
  //std::tie(r, a, c) = dpll(f5, det_prop, det_assign, std::chrono::seconds(1200), true);
  for (int i = 0; i < 100; i++) {
    std::cout << "\n======" << std::endl;
    std::tie(r, a, c) = dpll(f5, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200));
    REQUIRE(r == DPLLResult::Satisfiable);
  }
  std::cout << "DPLL on test_backjump.cnf" << " returned result " << r << " with assignment " << a << std::endl;

  // Larger (Einstein's Puzzle)
  CNFFormula ein_f = load_cnf("formulas/ein.cnf"); 
  std::tie(r, a, c) = dpll(ein_f, vsids_prop, uniform_random_assign);
  REQUIRE(r == DPLLResult::Satisfiable);
}
