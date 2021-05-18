#include <cannon/logic/read_dimacs_cnf.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cannon/logic/cnf.hpp>

using namespace cannon::logic;

CNFFormula cannon::logic::parse_cnf(const std::string& s) {
  std::string line;
  std::stringstream ss(s);
  CNFFormula f;
  unsigned int num_vars = 0;
  unsigned int num_clauses = 0;

  while(std::getline(ss, line)) {
    if (line[0] == 'c')
      continue;
    if (line[0] == 'p')
      if (line[2] == 'c' && line[3] == 'n' && line[4] == 'f') {
        std::size_t format_pos = line.find("cnf");
        line = line.erase(0, format_pos + 4);

        // Read num_vars
        std::string var_str = line.substr(0, line.find(" "));
        std::stringstream(var_str) >> num_vars;
        line = line.erase(0, line.find(" "));
        
        // Read num_clauses
        std::stringstream(line) >> num_clauses;

        break;
      }

    throw std::runtime_error("Found unrecognized DIMACS line in input");
  }

  std::string rest_str = ss.str();
  while (rest_str[0] == 'c')
    rest_str = rest_str.erase(0, rest_str.find_first_of("\n")+1);

  rest_str = rest_str.erase(0, rest_str.find('p'));
  rest_str = rest_str.erase(0, rest_str.find_first_of("\n")+1);
  for (unsigned int i = 0; i < num_clauses; i++) {
    auto c = parse_clause(rest_str);
    f.add_clause(std::move(c));
    rest_str = rest_str.erase(0, rest_str.find_first_not_of("0"));
  }

  if (f.get_num_props() != num_vars) {
    throw std::runtime_error("Parsed a different number of variables than were declared in input");
  }

  return f;
}

CNFFormula cannon::logic::load_cnf(const std::string& path) {
  try {
    std::ifstream fs(path);

    // Read everything into str
    std::string str((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());

    return parse_cnf(str);
  } catch (std::ifstream::failure e) {
    throw std::runtime_error("Couldn't open file for CNF formula");
  }
}

Clause cannon::logic::parse_clause(const std::string& s) { 
  std::string tmp_s = s;
  return parse_clause(tmp_s);
}

Clause cannon::logic::parse_clause(std::string& s) {
  Clause c;
  s = s.erase(0, s.find_first_not_of(" \t\r\n"));

  while (s.length() != 0 && s[0] != '0') {
    int var_num;
    bool negated = false;

    // Seek to first non-whitespace character
    s = s.erase(0, s.find_first_not_of(" \t\r\n"));
    std::string num_str = s.substr(0, s.find_first_of(" \t\r\n"));
    std::stringstream(num_str) >> var_num;

    // Handle negations
    if (var_num < 0) {
      var_num = std::abs(var_num);
      negated = true;
    }

    // We use 0-indexing, so subtract 1
    c.add_literal(var_num-1, negated);
    s = s.erase(0, s.find_first_of(" \t\r\n"));
    s = s.erase(0, s.find_first_not_of(" \t\r\n"));
  }
  
  return c;
}
