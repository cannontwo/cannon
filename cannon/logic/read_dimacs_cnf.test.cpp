#include <catch2/catch.hpp>

#include <cannon/logic/read_dimacs_cnf.hpp>
#include <cannon/logic/cnf.hpp>

using namespace cannon::logic;

TEST_CASE("ReadDimacsCNF", "[logic]") {
  // Test parsing clause
  std::string s1("1 3 -4 0");
  Clause c = parse_clause(s1);

  Clause test_c_1;
  test_c_1.add_literal(0, false);
  test_c_1.add_literal(2, false);
  test_c_1.add_literal(3, true);

  REQUIRE(s1.compare("0") == 0);
  REQUIRE(c == test_c_1);

  std::string s2("\t1 3\n -4 0");
  c = parse_clause(s2);

  REQUIRE(s2.compare("0") == 0);
  REQUIRE(c == test_c_1);

  std::string s3("2\n-3");
  c = parse_clause(s3);
  Clause test_c_2;
  test_c_2.add_literal(1, false);
  test_c_2.add_literal(2, true);
  REQUIRE(c == test_c_2);

  // Test parsing formula
  const std::string f_s1("c foobar\nc barfoo\np cnf 3 3\n1 2 0\n-2\t-1 0\n3 -1");
  CNFFormula f = parse_cnf(f_s1);

  CNFFormula test_f;
  test_f.add_clause(parse_clause("1 2 0"));
  test_f.add_clause(parse_clause("-2 -1 0"));
  test_f.add_clause(parse_clause("3 -1"));

  REQUIRE(f == test_f);

  // Test loading from file
  f = load_cnf("formulas/test.cnf"); 

  CNFFormula test_load_f;
  test_load_f.add_clause(parse_clause("1 3 -4"));
  test_load_f.add_clause(parse_clause("4"));
  test_load_f.add_clause(parse_clause("2 -3"));

  REQUIRE(f == test_load_f);
}
