#include "vector2_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <iostream>
#include <newton.hpp> // test power
#include <vector2.hpp>

using namespace std;
using namespace lineda;

typedef complex<double> Complex;
typedef vector2<vector2<double>> Recur;

CPPUNIT_TEST_SUITE_REGISTRATION(vector2_TestCase);

void vector2_TestCase::setUp() {
  _dd1 = vector2<double>(); // default constructor
  _dd2 = vector2<double>(4.);
  _dd3 = vector2<double>(2., 1.);
  _cd1 = vector2<Complex>();
  _cd2 = vector2<Complex>(_dd3);
  _cd3 = vector2<Complex>(Complex(2., 1.), Complex(8., 3.));
  _rd1 = Recur(_dd3);
  _rd2 = Recur();
  _rd3 = Recur(_dd2, _dd1);
  CPPUNIT_ASSERT_EQUAL(Recur(_dd3, _dd3), _rd1);
  // not Recur(_dd3.first, _dd3.second)
}

void vector2_TestCase::tearDown() {}

void vector2_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_dd1));
  CPPUNIT_ASSERT(copy_ok(_cd1));
  CPPUNIT_ASSERT(copy_ok(_rd1));
  CPPUNIT_ASSERT(copy_ok(_dd2));
  CPPUNIT_ASSERT(copy_ok(_cd2));
  CPPUNIT_ASSERT(copy_ok(_rd2));
  CPPUNIT_ASSERT(copy_ok(_dd3));
  CPPUNIT_ASSERT(copy_ok(_cd3));
  CPPUNIT_ASSERT(copy_ok(_rd3));
}

void vector2_TestCase::test_assign() {
  CPPUNIT_ASSERT(assign_ok(_dd1));
  CPPUNIT_ASSERT(assign_ok(_cd1));
  CPPUNIT_ASSERT(assign_ok(_rd1));
  CPPUNIT_ASSERT(assign_ok(_dd2));
  CPPUNIT_ASSERT(assign_ok(_cd2));
  CPPUNIT_ASSERT(assign_ok(_rd2));
  CPPUNIT_ASSERT(assign_ok(_dd3));
  CPPUNIT_ASSERT(assign_ok(_cd3));
  CPPUNIT_ASSERT(assign_ok(_rd3));
}

void vector2_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const vector2<int> v1 = vector2<int>::random();
    const vector2<int> v2 = vector2<int>::random();
    const vector2<int> v3 = vector2<int>::random();
    CPPUNIT_ASSERT(equal_to_ok(v1, v2, v3));
  }
  CPPUNIT_ASSERT(equal_to_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(equal_to_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(equal_to_ok(_rd1, _rd2, _rd3));
}

void vector2_TestCase::test_plus() {
  const vector2<int> v1(434343, 4344343);
  const vector2<int> v2(233232, 2323322);
  const vector2<int> v3(324343, 9656544);
  CPPUNIT_ASSERT(plus_ok(v1, v2, v3));
  CPPUNIT_ASSERT(plus_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(plus_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(plus_ok(_rd1, _rd2, _rd3));
}

void vector2_TestCase::test_multiplies() {
  // xxx for (size_t i=0; i<10; ++i) {
  // xxx   const vector2<int> v1 = vector2<int>::random();
  // xxx   const vector2<int> v2 = vector2<int>::random();
  // xxx   const vector2<int> v3 = vector2<int>::random();
  // xxx   CPPUNIT_ASSERT (multiplies1_ok(v1, v2, v3));
  // xxx }
  // xxx CPPUNIT_ASSERT (multiplies1_ok(_dd1, _dd2, _dd3));
  // xxx CPPUNIT_ASSERT (multiplies1_ok(_cd1, _cd2, _cd3));
  // xxx CPPUNIT_ASSERT (multiplies1_ok(_rd1, _rd2, _rd3));
}

void vector2_TestCase::test_power() {
  // CPPUNIT_ASSERT (power(_dd1,5) == _dd1*_dd1*_dd1*_dd1*_dd1);
  // CPPUNIT_ASSERT (power(_dd2,4) == _dd2*_dd2*_dd2*_dd2);
  // CPPUNIT_ASSERT (power(_dd3,3) == _dd3*_dd3*_dd3);
  // CPPUNIT_ASSERT (power(_cd1,5) == _cd1*_cd1*_cd1*_cd1*_cd1);
  // CPPUNIT_ASSERT (power(_cd2,4) == _cd2*_cd2*_cd2*_cd2);
  // CPPUNIT_ASSERT (power(_cd3,3) == _cd3*_cd3*_cd3);
  // CPPUNIT_ASSERT (power(_rd1,5) == _rd1*_rd1*_rd1*_rd1*_rd1);
  // CPPUNIT_ASSERT (power(_rd2,4) == _rd2*_rd2*_rd2*_rd2);
  // CPPUNIT_ASSERT (power(_rd3,3) == _rd3*_rd3*_rd3);
}
