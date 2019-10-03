#include "concept_test_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <newton.hpp>

using namespace std;
using namespace lineda;

CPPUNIT_TEST_SUITE_REGISTRATION(concept_test_TestCase);

void concept_test_TestCase::setUp() {
  _d1 = _CC(complex<double>(1, 2), complex<double>(3, 4));
  _d2 = _CC(complex<double>(5, 6), complex<double>(7, 8));
  _d3 = _CC(complex<double>(1, 3), complex<double>(5, 7));
}

void concept_test_TestCase::tearDown() {}

void concept_test_TestCase::test_copy_ok() {
  CPPUNIT_ASSERT(copy_ok(_d1));
  CPPUNIT_ASSERT(copy_ok(_d2));
  CPPUNIT_ASSERT(copy_ok(_d3));
}

void concept_test_TestCase::test_assign_ok() {
  CPPUNIT_ASSERT(assign_ok(_d1));
  CPPUNIT_ASSERT(assign_ok(_d2));
  CPPUNIT_ASSERT(assign_ok(_d3));
}

void concept_test_TestCase::test_equal_to_ok() {
  CPPUNIT_ASSERT(equal_to_ok(_d1, _d2, _d3));
}

void concept_test_TestCase::test_less_ok() {
  // Note: complex is not less_than comparable.
}

void concept_test_TestCase::test_plus_ok() {
  CPPUNIT_ASSERT(plus_ok(_d1, _d2, _d3));
}

void concept_test_TestCase::test_multiplies_ok() {
  CPPUNIT_ASSERT(multiplies1_ok(_d1, _d2, _d3));
}

void concept_test_TestCase::test_ring_ok() {
  /* Distributive law does not hold */
  CPPUNIT_ASSERT(
      !ring_ok(_d1, _d2, _d3, std::plus<_CC>(), std::multiplies<_CC>()));
}
