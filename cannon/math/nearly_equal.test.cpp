#include <cassert>

#include <cannon/math/nearly_equal.hpp>

// Tests from https://floating-point-gui.de/errors/NearlyEqualsTest.java

using namespace cannon::math;

int main() {
  // Big
  assert(nearly_equal(1000000.f, 1000001.f));
  assert(nearly_equal(1000001.f, 1000000.f));
  assert(!nearly_equal(10000.f, 10001.f));
  assert(!nearly_equal(10001.f, 10000.f));

  // Big & Negative
  assert(nearly_equal(-1000000.f, -1000001.f));
  assert(nearly_equal(-1000001.f, -1000000.f));
  assert(!nearly_equal(-10000.f, -10001.f));
  assert(!nearly_equal(-10001.f, -10000.f));

  // Numbers around 1
  assert(nearly_equal(1.0000001f, 1.0000002f));
  assert(nearly_equal(1.0000002f, 1.0000001f));
  assert(!nearly_equal(1.0002f, 1.0001f));
  assert(!nearly_equal(1.0001f, 1.0002f));

  // Numbers around -1
  assert(nearly_equal(-1.0000001f, -1.0000002f));
  assert(nearly_equal(-1.0000002f, -1.0000001f));
  assert(!nearly_equal(-1.0002f, -1.0001f));
  assert(!nearly_equal(-1.0001f, -1.0002f));

  // Numbers between 1 and 0
  assert(nearly_equal(0.000000001000001f, 0.000000001000002f));
  assert(nearly_equal(0.000000001000002f, 0.000000001000001f));
  assert(!nearly_equal(0.000000000001002f, 0.000000000001001f));
  assert(!nearly_equal(0.000000000001001f, 0.000000000001002f));

  // Small differences away from zero
  assert(nearly_equal(0.3f, 0.30000003f));
  assert(nearly_equal(-0.3f, -0.30000003f));

  // Comparisons involving zero
  assert(nearly_equal(0.0f, 0.0f));
  assert(nearly_equal(0.0f, -0.0f));
  assert(nearly_equal(-0.0f, -0.0f));
  assert(!nearly_equal(0.00000001f, 0.0f));
  assert(!nearly_equal(0.0f, 0.00000001f));
  assert(!nearly_equal(-0.00000001f, 0.0f));
  assert(!nearly_equal(0.0f, -0.00000001f));

  assert(nearly_equal(0.0f, 1e-40f, 0.01f));
  assert(nearly_equal(1e-40f, 0.0f, 0.01f));
  assert(!nearly_equal(1e-40f, 0.0f, 0.000001f));
  assert(!nearly_equal(0.0f, 1e-40f, 0.000001f));

  assert(nearly_equal(0.0f, -1e-40f, 0.1f));
  assert(nearly_equal(-1e-40f, 0.0f, 0.1f));
  assert(!nearly_equal(-1e-40f, 0.0f, 0.00000001f));
  assert(!nearly_equal(0.0f, -1e-40f, 0.00000001f));
}
