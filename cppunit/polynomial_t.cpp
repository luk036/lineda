#include "polynomial_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <newton.hpp>
#include <polynomial.hpp>

using namespace std;
using namespace lineda;

typedef complex<double> Complex;
typedef polynomial<polynomial<double>> Recur;

CPPUNIT_TEST_SUITE_REGISTRATION(polynomial_TestCase);

void polynomial_TestCase::setUp() {
  double ad1[] = {1., 2., 3., 0., 5.};
  double ad2[] = {0., 2., 4.};
  // double ad3[] = { 3., 1. };

  _dd1 = polynomial<double>(ad1, 5);
  _dd2 = polynomial<double>(ad2, 3);
  // d3 = polynomial<double>(ad3, 2);

  Complex acd1[] = {1., 2., 3., 0., 5.};
  Complex acd2[] = {2., 2., 1.};
  // Complex acd3[] = { 3., 1. };

  _cd1 = polynomial<Complex>(acd1, 5);
  _cd2 = polynomial<Complex>(acd2, 3);
  //_cd3 = polynomial<Complex>(acd3, 2);

  _rd1 = Recur(_dd1);
  _rd2 = Recur(_dd2);

  // polynomial<double> x = _rd1(1.2);
  // double y = _rd1(1.2)(2.3);
  // matrix2<double> z = md1(1.2);
  // matrix2<double> k = md1(z);
  // matrix2<double> k2 = d1.eval<matrix2<double>, matrix2<double> >(z);
}

void polynomial_TestCase::tearDown() {}

void polynomial_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_dd1));
  CPPUNIT_ASSERT(copy_ok(_cd1));
  CPPUNIT_ASSERT(copy_ok(_rd1));
}

void polynomial_TestCase::test_equal_to() {
  CPPUNIT_ASSERT(equal_to_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(equal_to_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(equal_to_ok(_rd1, _rd2, _rd3));
}

void polynomial_TestCase::test_less() {
  // Note: complex polynomial is not less_than comparable.
  CPPUNIT_ASSERT(less_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(less_ok(_rd1, _rd2, _rd3));
}

void polynomial_TestCase::test_plus() {
  CPPUNIT_ASSERT(plus_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(plus_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(plus_ok(_rd1, _rd2, _rd3));
}

void polynomial_TestCase::test_multiplies() {
  CPPUNIT_ASSERT(multiplies1_ok(_dd1, _dd2, _dd3));
  CPPUNIT_ASSERT(multiplies1_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(multiplies1_ok(_rd1, _rd2, _rd3));
}

void polynomial_TestCase::test_power() {
  CPPUNIT_ASSERT_EQUAL(_dd1 * _dd1 * _dd1 * _dd1 * _dd1, power(_dd1, 5));
  CPPUNIT_ASSERT_EQUAL(_dd2 * _dd2 * _dd2 * _dd2, power(_dd2, 4));
  CPPUNIT_ASSERT_EQUAL(_dd3 * _dd3 * _dd3, power(_dd3, 3));
  CPPUNIT_ASSERT_EQUAL(_cd1 * _cd1 * _cd1 * _cd1 * _cd1, power(_cd1, 5));
  CPPUNIT_ASSERT_EQUAL(_cd2 * _cd2 * _cd2 * _cd2, power(_cd2, 4));
  CPPUNIT_ASSERT_EQUAL(_cd3 * _cd3 * _cd3, power(_cd3, 3));
  CPPUNIT_ASSERT_EQUAL(_rd1 * _rd1 * _rd1 * _rd1 * _rd1, power(_rd1, 5));
  CPPUNIT_ASSERT_EQUAL(_rd2 * _rd2 * _rd2 * _rd2, power(_rd2, 4));
  CPPUNIT_ASSERT_EQUAL(_rd3 * _rd3 * _rd3, power(_rd3, 3));
}

void polynomial_TestCase::test_find_root() {
  double x = -0.51;
  bool found = _dd2.find_root(x, 100, 1e-10);
  CPPUNIT_ASSERT(found);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.5, x, 1e-8);

  Complex y = Complex(-0.9, 0.9);
  found = _cd2.find_root(y, 100, 1e-10);
  CPPUNIT_ASSERT(found);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(-1., y.real(), 1e-8);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1., y.imag(), 1e-8);
}

void polynomial_TestCase::test_interpolation() {
  AAF aad1[] = {1., 2., 3., 0., 5.};
  AAF aad2[] = {2., 5., interval(3.1, 4.1), 7., 2.};
  _aa1 = polynomial<AAF>(aad1, aad2, 5); // interpolation
  CPPUNIT_ASSERT_DOUBLES_EQUAL(_aa1(2).convert().mid(), 5., 1e-8);
}
