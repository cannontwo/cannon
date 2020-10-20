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
  for (int i = 0; i < props.size(); i++) {
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

int main() {
  DPLLResult r;
  Assignment a;

  // Empty
  CNFFormula f;
  std::tie(r, a) = dpll(f, uniform_random_prop, uniform_random_assign);
  log_info("DPLL returned result", r, "with assignment", a);
  assert(r == DPLLResult::Satisfiable);

  // Basic
  CNFFormula f1;
  f1.add_clause(parse_clause("1"));

  std::tie(r, a) = dpll(f1, uniform_random_prop, uniform_random_assign);
  log_info("DPLL returned result", r, "with assignment", a);
  assert(r == DPLLResult::Satisfiable);

  Assignment a1 = {PropAssignment::True};
  assert(a.size() == 1);
  assert(a[0] == a1[0]);

  // True and False
  CNFFormula f2;
  f2.add_clause(parse_clause("1"));
  f2.add_clause(parse_clause("-2"));

  std::tie(r, a) = dpll(f2, uniform_random_prop, uniform_random_assign);
  log_info("DPLL returned result", r, "with assignment", a);
  assert(r == DPLLResult::Satisfiable);

  Assignment a2 = {PropAssignment::True, PropAssignment::False};
  assert(a.size() == 2);
  assert(a[0] == a2[0] && a[1] == a2[1]);

  // Unsatisfiable
  CNFFormula f3;
  f3.add_clause(parse_clause("1"));
  f3.add_clause(parse_clause("-1"));

  std::tie(r, a) = dpll(f3, uniform_random_prop, uniform_random_assign);
  log_info("DPLL returned result", r, "with assignment", a);
  assert(r == DPLLResult::Unsatisfiable);

  // Larger (Einstein's Puzzle)
  CNFFormula ein_f = load_cnf("formulas/ein.cnf"); 
  log_info("Parsed Einstein's Puzzle as", ein_f);
  std::tie(r, a) = dpll(ein_f, uniform_random_prop, uniform_random_assign);
  log_info("DPLL on Einstein's Puzzle returned result", r, "with assignment", a);
  assert(r == DPLLResult::Satisfiable);

  Simplification s = std::valarray<bool>(false, 375);
  log_info("Evaluation of formula on returned assignment is");
  log_info(ein_f.eval(a, s));
}
