#include <catch2/catch.hpp>

#include <cannon/math/digits.hpp>

using namespace cannon::math;

TEST_CASE("Digits", "[math]") {
  auto seq_digits = get_digits(123456789);
  REQUIRE(seq_digits.size() == 9);
  REQUIRE(seq_digits[0] == 9);
  REQUIRE(seq_digits[1] == 8);
  REQUIRE(seq_digits[2] == 7);
  REQUIRE(seq_digits[3] == 6);
  REQUIRE(seq_digits[4] == 5);
  REQUIRE(seq_digits[5] == 4);
  REQUIRE(seq_digits[6] == 3);
  REQUIRE(seq_digits[7] == 2);
  REQUIRE(seq_digits[8] == 1);

  auto bin_digits = get_digits(8, 2);
  REQUIRE(bin_digits.size() == 4);
  REQUIRE(bin_digits[0] == 0);
  REQUIRE(bin_digits[1] == 0);
  REQUIRE(bin_digits[2] == 0);
  REQUIRE(bin_digits[3] == 1);
}
