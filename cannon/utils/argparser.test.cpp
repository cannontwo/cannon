#include <catch2/catch.hpp>

#include <cannon/utils/argparser.hpp>

using namespace cannon::utils;

TEST_CASE("ArgParser", "[utils]") {
  char prog_name[] = "foo";
  char arg1[] = "bar";

  char *test[2];
  test[0] = prog_name;
  test[1] = arg1;

  ArgParser p;
  p.parse(1, test);
  REQUIRE(p.get_inputs().empty());

  p.parse(2, test);
  REQUIRE(p.get_inputs().size() == 1);
  REQUIRE(p.get_inputs()[0] == "bar");
}
