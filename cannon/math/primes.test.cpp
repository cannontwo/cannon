#include <catch2/catch.hpp>

#include <cannon/math/primes.hpp>

using namespace cannon::math;

TEST_CASE("Primes", "[math]") {
  auto primes = get_primes_up_to(20); 

  REQUIRE(primes.size() == 8);
  REQUIRE(primes[0] == 2);
  REQUIRE(primes[1] == 3);
  REQUIRE(primes[2] == 5);
  REQUIRE(primes[3] == 7);
  REQUIRE(primes[4] == 11);
  REQUIRE(primes[5] == 13);
  REQUIRE(primes[6] == 17);
  REQUIRE(primes[7] == 19);
  
  primes = get_primes_up_to(2);
  
  REQUIRE(primes.size() == 1);
  REQUIRE(primes[0] == 2);

  auto factor = get_largest_prime_factor(13195);
  REQUIRE(factor == 29);

  factor = get_largest_prime_factor(2);
  REQUIRE(factor == 2);

  factor = get_largest_prime_factor(4);
  REQUIRE(factor == 2);

  auto prime_factors = get_prime_factorization(20);
  REQUIRE(prime_factors.size() == 3);
  REQUIRE(prime_factors.count(2) == 2);
  REQUIRE(prime_factors.count(5) == 1);

  prime_factors = get_prime_factorization(100);
  REQUIRE(prime_factors.size() == 4);
  REQUIRE(prime_factors.count(2) == 2);
  REQUIRE(prime_factors.count(5) == 2);

  REQUIRE(is_prime(5));
  REQUIRE(is_prime(1601));
  REQUIRE(!is_prime(4));
  REQUIRE(!is_prime(10));
}
