#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <cannon/logic/cnf.hpp>
#include <cannon/logic/dpll.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

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
  std::tie(r, a, c) = dpll(f, uniform_random_prop, uniform_random_assign);
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
  //log_info("Evaluation of formula on returned assignment is");
  //log_info(f.eval(a, s));

  //os << "Evaluation of formula on returned assignment is " << f.eval(a, s);
  //os.close();
}
