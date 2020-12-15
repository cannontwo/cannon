#include <catch2/catch.hpp>

#include <cannon/logic/cnf.hpp>
#include <cannon/logic/write_dimacs_cnf.hpp>
#include <cannon/logic/read_dimacs_cnf.hpp>

using namespace cannon::logic;

TEST_CASE("WriteDimacsCNF", "[logic]") {
  const std::string f_s("c foobar\nc barfoo\np cnf 3 3\n1 2 0\n-2\t-1 0\n3 -1");
  CNFFormula f = parse_cnf(f_s);

  auto r_s = write_cnf(f);
  REQUIRE(r_s.compare("p cnf 3 3\n1 2 0\n-1 -2 0\n-1 3 0\n") == 0);
}
