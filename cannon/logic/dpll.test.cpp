#include <catch2/catch.hpp>

#include <random>

#include <cannon/logic/dpll.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>

using namespace cannon::logic;

std::vector<double> uniform_random_prop(const CNFFormula& form, 
    const Assignment& a,const Simplification& s, 
    const std::vector<unsigned int>& props) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> d;

  std::vector<double> ret_vec;
  ret_vec.reserve(props.size());
  for (unsigned int i = 0; i < props.size(); i++) {
    ret_vec.push_back(d(gen));
  }

  return ret_vec;
}

bool uniform_random_assign(const CNFFormula& form, const Assignment& a, 
    const Simplification& s, unsigned int prop) {
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
  std::tie(r, a, c) = dpll(f, uniform_random_prop, uniform_random_assign);
  REQUIRE(r == DPLLResult::Satisfiable);

  // Basic
  CNFFormula f1;
  f1.add_clause(parse_clause("1"));

  std::tie(r, a, c) = dpll(f1, uniform_random_prop, uniform_random_assign);
  REQUIRE(r == DPLLResult::Satisfiable);

  Assignment a1(1);
  a1[0] = PropAssignment::True;
  REQUIRE(a.size() == 1);
  //REQUIRE(a[0] == a1[0]);

  // True and False
  CNFFormula f2;
  f2.add_clause(parse_clause("1"));
  f2.add_clause(parse_clause("-2"));

  std::tie(r, a, c) = dpll(f2, uniform_random_prop, uniform_random_assign);
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

  std::tie(r, a, c) = dpll(f3, uniform_random_prop, uniform_random_assign);
  REQUIRE(r == DPLLResult::Unsatisfiable);

  // Requiring splitting
  CNFFormula f4;
  f4.add_clause(parse_clause("-1 2"));
  f4.add_clause(parse_clause("-2 3"));
  f4.add_clause(parse_clause("-3 4"));
  f4.add_clause(parse_clause("-4 1"));
  f4.add_clause(parse_clause("1 2 3 4"));

  std::tie(r, a, c) = dpll(f4, uniform_random_prop, uniform_random_assign);
  REQUIRE(r == DPLLResult::Satisfiable);

  // Larger (Einstein's Puzzle)
  //CNFFormula ein_f = load_cnf("formulas/ein.cnf"); 
  //log_info("Parsed Einstein's Puzzle as", ein_f);
  //std::tie(r, a) = dpll(ein_f, uniform_random_prop, uniform_random_assign);
  //log_info("DPLL on Einstein's Puzzle returned result", r, "with assignment", a);
  //REQUIRE(r == DPLLResult::Satisfiable);

  //Simplification s = std::valarray<bool>(false, 375);
  //log_info("Evaluation of formula on returned assignment is");
  //log_info(ein_f.eval(a, s));
}
