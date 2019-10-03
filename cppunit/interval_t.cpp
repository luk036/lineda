#include "interval_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <interval.hpp>
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(interval_TestCase);

using namespace std;
using namespace lineda;
using namespace boost;
using namespace numeric;

// xxx typedef rational<int> Rat;
typedef interval<interval<double>> Recur;
typedef interval<int> iint_t;
typedef interval<unsigned int> iuint_t;

void interval_TestCase::setUp() {
  iint_t ii(-2, 3); // [lower .. upper]

  _dd1 = interval<double>();    // default constructor
  _dd2 = interval<double>(4.0); // an exact number
  _dd3 = interval<double>(ii);  // from another interval
  // xxx _cd1 = interval<Rat>();                      // default constructor
  // xxx _cd2 = interval<Rat>(Rat(2, 3));             // an exact number
  // xxx _cd3 = interval<Rat>(Rat(-1,2), Rat(2, 1));  // [lower .. upper]
  _rd1 = Recur();           // default constructor
  _rd2 = Recur(_dd3);       // an "exact" number
  _rd3 = Recur(_dd1, _dd2); // [lower .. upper]
}

void interval_TestCase::tearDown() {}

void interval_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_dd1));
  // CPPUNIT_ASSERT (copy_ok(_cd1));
  CPPUNIT_ASSERT(copy_ok(_rd1));
  CPPUNIT_ASSERT(copy_ok(_dd2));
  // CPPUNIT_ASSERT (copy_ok(_cd2));
  CPPUNIT_ASSERT(copy_ok(_rd2));
  CPPUNIT_ASSERT(copy_ok(_dd3));
  // CPPUNIT_ASSERT (copy_ok(_cd3));
  CPPUNIT_ASSERT(copy_ok(_rd3));
}

void interval_TestCase::test_assign() {
  CPPUNIT_ASSERT(assign_ok(_dd1));
  // CPPUNIT_ASSERT (assign_ok(_cd1));
  CPPUNIT_ASSERT(assign_ok(_rd1));
  CPPUNIT_ASSERT(assign_ok(_dd2));
  // CPPUNIT_ASSERT (assign_ok(_cd2));
  CPPUNIT_ASSERT(assign_ok(_rd2));
  CPPUNIT_ASSERT(assign_ok(_dd3));
  // CPPUNIT_ASSERT (assign_ok(_cd3));
  CPPUNIT_ASSERT(assign_ok(_rd3));
}

void interval_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const iint_t i1 = iint_t::random();
    const iint_t i2 = iint_t::random();
    const iint_t i3 = iint_t::random();
    CPPUNIT_ASSERT(equal_to_ok(i1, i2, i3));
  }
  CPPUNIT_ASSERT(equal_to_ok(_dd1, _dd2, _dd3));
  // CPPUNIT_ASSERT (equal_to_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(equal_to_ok(_rd1, _rd2, _rd3));
}

void interval_TestCase::test_less() {
  for (size_t i = 0; i < 10; ++i) {
    const iint_t i1 = iint_t::random();
    const iint_t i2 = iint_t::random();
    const iint_t i3 = iint_t::random();
    CPPUNIT_ASSERT(less_ok(i1, i2, i3));
  }
  CPPUNIT_ASSERT(less_ok(_dd1, _dd2, _dd3));
  // CPPUNIT_ASSERT (less_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(less_ok(_rd1, _rd2, _rd3));
}

void interval_TestCase::test_plus() {
  for (size_t i = 0; i < 10; ++i) {
    const iint_t i1 = iint_t::random();
    const iint_t i2 = iint_t::random();
    const iint_t i3 = iint_t::random();
    CPPUNIT_ASSERT(plus_ok(i1, i2, i3));
  }
  CPPUNIT_ASSERT_EQUAL(interval<double>(2, 7), _dd1 + _dd2 + _dd3);
  // xxx CPPUNIT_ASSERT_EQUAL(interval<Rat>(Rat(1,6), Rat(8,3)),
  // _cd1+_cd2+_cd3);
  CPPUNIT_ASSERT_EQUAL(Recur(interval<double>(-2, 3), interval<double>(2, 7)),
                       _rd1 + _rd2 + _rd3);
}

void interval_TestCase::test_multiplies() {
  size_t i;

  for (i = 0; i < 100; ++i) {
    const iint_t i1 = iint_t::random();
    const iint_t i2 = iint_t::random();
    const iint_t i3 = iint_t::random();
    CPPUNIT_ASSERT(commutative_ok(i1, i2, i3, std::multiplies<iint_t>()));
    CPPUNIT_ASSERT(associative_ok(i1, i2, i3, std::multiplies<iint_t>()));
    // Does not satisfy distributive law!!!
  }

  // Unsigned int
  for (i = 0; i < 100; ++i) {
    const iuint_t i1 = iuint_t::random();
    const iuint_t i2 = iuint_t::random();
    const iuint_t i3 = iuint_t::random();
    CPPUNIT_ASSERT(multiplies1_ok(i1, i2, i3));
  }

  CPPUNIT_ASSERT_EQUAL(interval<double>(-8, 12), _dd2 * _dd3);
  // xxx CPPUNIT_ASSERT_EQUAL(interval<Rat>(Rat(-1,3), Rat(4,3)), _cd2*_cd3);
  CPPUNIT_ASSERT_EQUAL(Recur(interval<double>(-8, 12), interval<double>(0, 0)),
                       _rd2 * _rd3);
}

void interval_TestCase::test_intersect() {
  for (size_t i = 0; i < 100; ++i) {
    const iint_t i1 = iint_t::random();
    const iint_t i2 = iint_t::random();
    CPPUNIT_ASSERT(i1.intersect(i1) == i1);
    CPPUNIT_ASSERT(!i1.overlap(i2) || i1.intersect(i2) == i2.intersect(i1));
  }

  CPPUNIT_ASSERT(_dd1.intersect(_dd1) == _dd1);
  // CPPUNIT_ASSERT (_cd1.intersect(_cd1) == _cd1);
  // CPPUNIT_ASSERT (_rd1.intersect(_rd1) == _rd1);
  CPPUNIT_ASSERT(_dd2.intersect(_dd2) == _dd2);
  // CPPUNIT_ASSERT (_cd2.intersect(_cd2) == _cd2);
  // CPPUNIT_ASSERT (_rd2.intersect(_rd2) == _rd2);
  CPPUNIT_ASSERT(_dd3.intersect(_dd3) == _dd3);
  // CPPUNIT_ASSERT (_cd3.intersect(_cd3) == _cd3);
  // CPPUNIT_ASSERT (_rd3.intersect(_rd3) == _rd3);

  // CPPUNIT_ASSERT_EQUAL (_cd2, _cd2.intersect(_cd3));
  // CPPUNIT_ASSERT_EQUAL (_rd2, _rd2.intersect(_rd3));
}
