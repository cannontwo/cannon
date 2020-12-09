#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <Eigen/Dense>

#include <cannon/logic/cnf.hpp>
#include <cannon/logic/dpll.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::logic;
using namespace cannon::log;

std::vector<double> uniform_random_prop(const CNFFormula& form, const
    Assignment& a,const Simplification& s, const std::vector<unsigned int>&
    props, std::vector<std::vector<unsigned int>> watched, const
    VectorXd& vsids) {
  std::random_device rd;
  std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d(0.0, 1.0);

  std::vector<double> ret_vec;
  ret_vec.reserve(props.size());
  for (unsigned int i = 0; i < props.size(); i++) {
    ret_vec.push_back(d(gen));
  }

  return ret_vec;
}

std::vector<double> two_clause_prop(const CNFFormula& form, const Assignment&
    a,const Simplification& s, const std::vector<unsigned int>& props,
    std::vector<std::vector<unsigned int>> watched, const VectorXd&
    vsids) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  auto clause_num_vec = form.get_num_two_clauses(a, s, props);
  unsigned int max_two_clauses = *std::max_element(clause_num_vec.begin(), clause_num_vec.end());

  std::vector<double> ret_vec;
  ret_vec.reserve(props.size());
  for (unsigned int i = 0; i < props.size(); i++) {
    if (clause_num_vec[i] == max_two_clauses)
      ret_vec.push_back(d(gen));
    else
      ret_vec.push_back(-1.0);
  }

  return ret_vec;
}

std::vector<double> adj_mat_eig_prop(const CNFFormula& form, 
    const Assignment& a,const Simplification& s, 
    const std::vector<unsigned int>& props, std::vector<std::vector<unsigned int>> watched,
    const VectorXd& vsids) {
  MatrixXd adj_mat = form.make_adjacency_mat(a, s);
  SelfAdjointEigenSolver<MatrixXd> solver(adj_mat);

  if (solver.info() != Success) {
    log_info("Couldn't compute eigenvalues");
    return uniform_random_prop(form, a, s, props, watched, vsids);
  }
  VectorXd largest_eigenvector = solver.eigenvectors().col(form.get_num_props() - 1);

  // Elements of the eigenvector corresponding to the largest eigenvalue of the
  // adjacency matrix have the greatest centrality to the graph formed by the
  // CNF clauses.
  std::vector<double> ret_vec;
  ret_vec.reserve(props.size());
  for (auto &p : props) {
    ret_vec.push_back(largest_eigenvector[p]);
  }

  return ret_vec;
}

std::vector<double> vsids_prop(const CNFFormula& form, const Assignment&
    a,const Simplification& s, const std::vector<unsigned int>& props,
    std::vector<std::vector<unsigned int>> watched, const VectorXd& vsids) {
  std::vector<double> vsids_vec;

  for (auto &prop : props) {
    vsids_vec.push_back(vsids[prop]);
  }

  return vsids_vec;
}


bool uniform_random_assign(const CNFFormula& form, const Assignment& a, 
    const Simplification& s, unsigned int prop, std::vector<std::vector<unsigned int>> watched) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  double t = d(gen);
  return t < 0.5;
}

bool max_vote_assign(const CNFFormula& form, const Assignment& a, const
    Simplification& s, unsigned int prop, std::vector<std::vector<unsigned int>> watched) {
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
  log_info("Parsed passed CNF file as", f);
  os << "Parsed passed CNF file " << filename << " as " << std::endl << f << std::endl << std::endl;

  auto start = std::chrono::steady_clock::now();
  //std::tie(r, a, c) = dpll(f, uniform_random_prop, uniform_random_assign); // Random heuristic
  //std::tie(r, a, c) = dpll(f, two_clause_prop, uniform_random_assign); // 2-clause heuristic
  //std::tie(r, a, c) = dpll(f, two_clause_prop, max_vote_assign); // custom heuristic
  std::tie(r, a, c) = dpll(f, vsids_prop, max_vote_assign); // custom heuristic
  auto end = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  y_os << "  time_micros: " << std::to_string(duration.count()) << std::endl;
  
  log_info("DPLL recursively called", c, "times");
  os << "DPLL recursively called " << c << " times" << std::endl << std::endl;
  y_os << "  calls: " << std::to_string(c) << std::endl;
  
  log_info("DPLL on", filename, "returned result", r, "with assignment", a);
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

  // TODO Report number of DPLL calls as well

  Simplification s = std::valarray<bool>(false, f.get_num_props());
  log_info("Evaluation of formula on returned assignment is");
  log_info(f.eval(a, s));

  os << "Evaluation of formula on returned assignment is " << f.eval(a, s);
  os.close();
}
