#include <catch2/catch.hpp>

#include <cannon/math/nearly_equal.hpp>

// Tests from https://floating-point-gui.de/errors/NearlyEqualsTest.java

using namespace cannon::math;

TEST_CASE("NearlyEqual", "[math]") {
  // Big
  REQUIRE(nearly_equal(1000000.f, 1000001.f));
  REQUIRE(nearly_equal(1000001.f, 1000000.f));
  REQUIRE(!nearly_equal(10000.f, 10001.f));
  REQUIRE(!nearly_equal(10001.f, 10000.f));

  // Big & Negative
  REQUIRE(nearly_equal(-1000000.f, -1000001.f));
  REQUIRE(nearly_equal(-1000001.f, -1000000.f));
  REQUIRE(!nearly_equal(-10000.f, -10001.f));
  REQUIRE(!nearly_equal(-10001.f, -10000.f));

  // Numbers around 1
  REQUIRE(nearly_equal(1.0000001f, 1.0000002f));
  REQUIRE(nearly_equal(1.0000002f, 1.0000001f));
  REQUIRE(!nearly_equal(1.0002f, 1.0001f));
  REQUIRE(!nearly_equal(1.0001f, 1.0002f));

  // Numbers around -1
  REQUIRE(nearly_equal(-1.0000001f, -1.0000002f));
  REQUIRE(nearly_equal(-1.0000002f, -1.0000001f));
  REQUIRE(!nearly_equal(-1.0002f, -1.0001f));
  REQUIRE(!nearly_equal(-1.0001f, -1.0002f));

  // Numbers between 1 and 0
  REQUIRE(nearly_equal(0.000000001000001f, 0.000000001000002f));
  REQUIRE(nearly_equal(0.000000001000002f, 0.000000001000001f));
  REQUIRE(!nearly_equal(0.000000000001002f, 0.000000000001001f));
  REQUIRE(!nearly_equal(0.000000000001001f, 0.000000000001002f));

  // Small differences away from zero
  REQUIRE(nearly_equal(0.3f, 0.30000003f));
  REQUIRE(nearly_equal(-0.3f, -0.30000003f));

  // Comparisons involving zero
  REQUIRE(nearly_equal(0.0f, 0.0f));
  REQUIRE(nearly_equal(0.0f, -0.0f));
  REQUIRE(nearly_equal(-0.0f, -0.0f));
  REQUIRE(!nearly_equal(0.00000001f, 0.0f));
  REQUIRE(!nearly_equal(0.0f, 0.00000001f));
  REQUIRE(!nearly_equal(-0.00000001f, 0.0f));
  REQUIRE(!nearly_equal(0.0f, -0.00000001f));

  REQUIRE(nearly_equal(0.0f, 1e-40f, 0.01f));
  REQUIRE(nearly_equal(1e-40f, 0.0f, 0.01f));
  REQUIRE(!nearly_equal(1e-40f, 0.0f, 0.000001f));
  REQUIRE(!nearly_equal(0.0f, 1e-40f, 0.000001f));

  REQUIRE(nearly_equal(0.0f, -1e-40f, 0.1f));
  REQUIRE(nearly_equal(-1e-40f, 0.0f, 0.1f));
  REQUIRE(!nearly_equal(-1e-40f, 0.0f, 0.00000001f));
  REQUIRE(!nearly_equal(0.0f, -1e-40f, 0.00000001f));
}
