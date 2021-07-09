#include <catch2/catch.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/math/bignum.hpp>

using namespace cannon::log;
using namespace cannon::math;

TEST_CASE("Bignum", "[math]") {
  BigUnsigned init_test(123);
  REQUIRE(init_test.size() == 3);
  REQUIRE(init_test.get_digits()[2] == 1);
  REQUIRE(init_test.get_digits()[1] == 2);
  REQUIRE(init_test.get_digits()[0] == 3);

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

  BigUnsigned threes("3");
  threes = threes * 3;
  REQUIRE(threes.size() == 1);
  REQUIRE(threes.get_digits()[0] == 9);

  threes = threes * 3;
  REQUIRE(threes.size() == 2);
  REQUIRE(threes.get_digits()[0] == 7);
  REQUIRE(threes.get_digits()[1] == 2);

  BigUnsigned foo(999);
  foo *= foo;
  REQUIRE(foo.size() == 6);
  REQUIRE(foo.get_digits()[0] == 1);
  REQUIRE(foo.get_digits()[1] == 0);
  REQUIRE(foo.get_digits()[2] == 0);
  REQUIRE(foo.get_digits()[3] == 8);
  REQUIRE(foo.get_digits()[4] == 9);
  REQUIRE(foo.get_digits()[5] == 9);

  foo *= 7;
  REQUIRE(foo.size() == 7);
  REQUIRE(foo.get_digits()[0] == 7);
  REQUIRE(foo.get_digits()[1] == 0);
  REQUIRE(foo.get_digits()[2] == 0);
  REQUIRE(foo.get_digits()[3] == 6);
  REQUIRE(foo.get_digits()[4] == 8);
  REQUIRE(foo.get_digits()[5] == 9);
  REQUIRE(foo.get_digits()[6] == 6);
  

  BigUnsigned eq_test_1("123");
  REQUIRE(!(eq_test_1 < eq_test_1));

  BigUnsigned eq_test_2("124");
  REQUIRE(eq_test_1 < eq_test_2);
  REQUIRE(!(eq_test_2 < eq_test_1));

}
