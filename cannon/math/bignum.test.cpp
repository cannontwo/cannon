#include <catch2/catch.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

TEST_CASE("Bignum", "[math]") {
  BigUnsigned a("2");
  BigUnsigned b("2");

  auto c = a + b;
  REQUIRE(c.size() == 1);
  REQUIRE(c.get_digits()[0] == 4);

  BigUnsigned d("55");
  BigUnsigned e("5");
  auto f = d + e;
  REQUIRE(f.size() == 2);
  REQUIRE(f.get_digits()[0] == 0);
  REQUIRE(f.get_digits()[1] == 6);

  BigUnsigned g("99");
  BigUnsigned h("1");
  auto i = g + h;
  REQUIRE(i.size() == 3);
  REQUIRE(i.get_digits()[0] == 0);
  REQUIRE(i.get_digits()[1] == 0);
  REQUIRE(i.get_digits()[2] == 1);

  BigUnsigned two_powers("2");
  for (unsigned int j = 2; j <= 15; j++) {
    two_powers = two_powers + two_powers;
    log_info("2^", j, "=", two_powers);
  }

  REQUIRE(two_powers.size() == 5);
  REQUIRE(two_powers.get_digits()[0] == 8);
  REQUIRE(two_powers.get_digits()[1] == 6);
  REQUIRE(two_powers.get_digits()[2] == 7);
  REQUIRE(two_powers.get_digits()[3] == 2);
  REQUIRE(two_powers.get_digits()[4] == 3);
}
