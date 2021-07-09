#include <catch2/catch.hpp>

#include <cannon/math/digits.hpp>
#include <cannon/math/palindromes.hpp>

using namespace cannon::math;

TEST_CASE("Palindromes", "[math]") {
  REQUIRE(is_palindrome(get_digits(1001)));
  REQUIRE(is_palindrome(get_digits(101)));
  REQUIRE(is_palindrome(get_digits(9, 2)));

  auto pals_10 = generate_palindromes_of_length(3);
  REQUIRE(pals_10.size() == 90);

  auto pals_2 = generate_palindromes_of_length(3, 2);
  REQUIRE(pals_2.size() == 2);

  auto pals = generate_palindromes(3);
  REQUIRE(pals.size() == 108);
}
