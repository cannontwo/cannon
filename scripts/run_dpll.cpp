#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <Eigen/Dense>

#include <cannon/logic/cnf.hpp>
#include <cannon/logic/dpll.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>


using namespace Eigen;

using namespace cannon::logic;


double calculate_moms(const CNFFormula& form, const Assignment& a, const
    Simplification& s, unsigned int prop) {
  unsigned int num_smallest_pos = 0; 
  unsigned int num_smallest_neg = 0; 
  unsigned int smallest_clause_size = form.get_smallest_clause_size(a, s);

  for (unsigned int i = 0; i < form.clauses_.size(); i++) {
    if (s[i])
      continue;

    if (form.clauses_[i].contains_prop(a, prop)) {
      if (form.clauses_[i].size(a) == smallest_clause_size) {
        if (form.clauses_[i].has_pos_literal(a, prop)) {
          num_smallest_pos += 1;
        } else {
          num_smallest_neg += 1;
        }
      }
    }
  }

  double first_part = (double)(num_smallest_pos + num_smallest_neg) * std::pow(2.0, 3.0);
  double second_part = num_smallest_pos * num_smallest_neg;

  return first_part + second_part;
}

unsigned int uniform_random_prop(const DPLLState& ds,
    const Assignment& a, const Simplification& s, std::vector<unsigned int>& props) {
  RandomComparator comp(ds);
  std::sort(props.begin(), props.end(), comp);

  return props[props.size() - 1];
}

unsigned int two_clause_prop(const DPLLState& ds,
    const Assignment& a, const Simplification& s, std::vector<unsigned int>& props) {
  RandomComparator rc(ds);
  TwoClauseComparator tc(ds, a, s, props, rc);

  std::sort(props.begin(), props.end(), tc);
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

bool uniform_random_assign(const CNFFormula& form, const Assignment& a, 
    const Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  double t = d(gen);
  return t < 0.5;
}

bool max_vote_assign(const CNFFormula& form, const Assignment& a, const
    Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  int num_pos = 0;
  int num_neg = 0;
  for (unsigned int c_num : watched[prop]) {
    if (s[c_num])
      continue;

    for (auto &l : form.clauses_[c_num].literals_) {
      if (l.prop_ == prop && l.negated_)
        num_neg += 1;

      if (l.prop_ == prop && !l.negated_)
        num_pos += 1;
    }
  }

  if (num_pos == num_neg) {
    double t = d(gen);
    return t < 0.5;
  } else {
    return num_pos > num_neg;
  }
}

bool min_vote_assign(const CNFFormula& form, const Assignment& a, const
    Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  int num_pos = 0;
  int num_neg = 0;
  for (unsigned int c_num : watched[prop]) {
    if (s[c_num])
      continue;

    for (auto &l : form.clauses_[c_num].literals_) {
      if (l.prop_ == prop && l.negated_)
        num_neg += 1;

      if (l.prop_ == prop && !l.negated_)
        num_pos += 1;
    }
  }

  if (num_pos == num_neg) {
    double t = d(gen);
    return t < 0.5;
  } else {
    return num_pos < num_neg;
  }
}

int main(int argc, char **argv) {
  DPLLResult r;
  Assignment a;
  int c;

  assert(argc >= 2);
  std::string filename(argv[1]);
  std::string filename_copy(filename);

  std::string report_filename =
    filename_copy.erase(filename_copy.find_first_of("."),
        filename_copy.size()-1);

  std::string yaml_report_filename(report_filename);
  report_filename += "_report.txt";
  yaml_report_filename += "_report.yaml";

  std::ofstream os(report_filename);
  std::ofstream y_os(yaml_report_filename); // Write to YAML as well

  y_os << "stats:" << std::endl;

  CNFFormula f = load_cnf(filename); 
  std::cout << "Parsed passed CNF file as " << f << std::endl;
  os << "Parsed passed CNF file " << filename << " as " << std::endl << f << std::endl << std::endl;

  auto start = std::chrono::steady_clock::now();
  //std::tie(r, a, c) = dpll(f, uniform_random_prop, uniform_random_assign, std::chrono::seconds(1200)); // Random heuristic
  //std::tie(r, a, c) = dpll(f, two_clause_prop, uniform_random_assign); // 2-clause heuristic
  std::tie(r, a, c) = dpll(f, vsids_prop, min_vote_assign, std::chrono::seconds(1200)); // custom heuristic
  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  y_os << "  time_micros: " << std::to_string(duration.count()) << std::endl;
  
  std::cout << "DPLL recursively called " << c << " times" << std::endl;
  os << "DPLL recursively called " << c << " times" << std::endl << std::endl;
  y_os << "  calls: " << std::to_string(c) << std::endl;
  
  std::cout << "DPLL on " << filename << " returned result " << r << " with assignment " << a << std::endl;
  os << "DPLL returned result " << r << " with assignment " << a << std::endl << std::endl;

  y_os << "  sat: ";
  if (r == DPLLResult::Satisfiable)
    y_os << "true" << std::endl;
  else 
    y_os << "false" << std::endl;

  y_os << "  cutoff: ";
  if (r == DPLLResult::Unknown) 
    y_os << "true" << std::endl;
  else
    y_os << "false" << std::endl;

  Simplification s = std::valarray<bool>(false, f.get_num_clauses());
  if (r == DPLLResult::Satisfiable) {
    std::cout << "Evaluation of formula on returned assignment is" << std::endl;
    std::cout << f.eval(a, s) << std::endl;
    assert(f.eval(a, s) == PropAssignment::True);

    os << "Evaluation of formula on returned assignment is " << f.eval(a, s);
    os.close();
  }
}
