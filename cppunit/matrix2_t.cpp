#include "matrix2_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <matrix2.hpp>
#include <newton.hpp> // test power

CPPUNIT_TEST_SUITE_REGISTRATION(matrix2_TestCase);

using namespace std;
using namespace lineda;
using std::complex;
using numeric::matrix2;

typedef complex<double> Complex;

void matrix2_TestCase::setUp() {
  _m0 = matrix2<int>();
  _m1 = matrix2<int>(34, 90, 23, 34);
  _m2 = matrix2<int>(44, 33, 33, 33);
  _m3 = matrix2<int>(43, 31, 93, 43);

  _dd0 = matrix2<double>();
  _dd1 = matrix2<double>(1., 2., 3., 4.);
  _dd2 = matrix2<double>(4., 5., 2., 1.);
  _dd3 = matrix2<double>(2., 1., 2., 2.);
  _cd1 = matrix2<Complex>();
  _cd2 = matrix2<Complex>(Complex(4., 5.), Complex(0.5, 12.5), Complex(1., 1.),
                          Complex(3., 1.));
  _cd3 = matrix2<Complex>(Complex(2., 1.), Complex(8., 3.), Complex(1., 8.),
                          Complex(4., 4.));
}

void matrix2_TestCase::tearDown() {}

void matrix2_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_dd1));
  CPPUNIT_ASSERT(copy_ok(_cd1));
  CPPUNIT_ASSERT(copy_ok(_dd2));
  CPPUNIT_ASSERT(copy_ok(_cd2));
  CPPUNIT_ASSERT(copy_ok(_dd3));
  CPPUNIT_ASSERT(copy_ok(_cd3));
}

void matrix2_TestCase::test_assign() {
  CPPUNIT_ASSERT(assign_ok(_dd1));
  CPPUNIT_ASSERT(assign_ok(_cd1));
  CPPUNIT_ASSERT(assign_ok(_dd2));
  CPPUNIT_ASSERT(assign_ok(_cd2));
  CPPUNIT_ASSERT(assign_ok(_dd3));
  CPPUNIT_ASSERT(assign_ok(_cd3));
}

void matrix2_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const matrix2<int> m1 = matrix2<int>::random();
    const matrix2<int> m2 = matrix2<int>::random();
    const matrix2<int> m3 = matrix2<int>::random();
    CPPUNIT_ASSERT(equal_to_ok(m1, m2, m3));
  }
  CPPUNIT_ASSERT(equal_to_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(equal_to_ok(_cd1, _cd2, _cd3));
}

void matrix2_TestCase::test_plus() {
  CPPUNIT_ASSERT(plus_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(plus_ok(_cd1, _cd2, _cd3));
  // CPPUNIT_ASSERT (plus_ok(_m0, _m0, _m0));
}

void matrix2_TestCase::test_multiplies() {
  // Note: matrix multiplication is not commutative
  CPPUNIT_ASSERT(multiplies2_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(multiplies2_ok(_cd1, _cd2, _cd3));
  // CPPUNIT_ASSERT (multiplies2_ok(_m1, _m2, _m3));
}

void matrix2_TestCase::test_power() {
  for (size_t i = 0; i < 10; ++i) {
    const matrix2<int> m = matrix2<int>::random();
    CPPUNIT_ASSERT_EQUAL(m * m * m, power(m, 3));
  }
  CPPUNIT_ASSERT(power(_dd1, 5) == _dd1 * _dd1 * _dd1 * _dd1 * _dd1);
  CPPUNIT_ASSERT(power(_dd2, 4) == _dd2 * _dd2 * _dd2 * _dd2);
  CPPUNIT_ASSERT(power(_dd3, 3) == _dd3 * _dd3 * _dd3);
  CPPUNIT_ASSERT(power(_cd1, 5) == _cd1 * _cd1 * _cd1 * _cd1 * _cd1);
  CPPUNIT_ASSERT(power(_cd2, 4) == _cd2 * _cd2 * _cd2 * _cd2);
  CPPUNIT_ASSERT(power(_cd3, 3) == _cd3 * _cd3 * _cd3);
}
