#ifndef CPP_UNIT_POLYNOMIAL_HPP
#define CPP_UNIT_POLYNOMIAL_HPP

#include <aa.h>
#include <complex>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <matrix2.hpp>
#include <polynomial.hpp>

using std::complex;
using namespace numeric;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for polynomial
 */
class polynomial_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(polynomial_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_less);
  CPPUNIT_TEST(test_plus);
  CPPUNIT_TEST(test_multiplies);
  CPPUNIT_TEST(test_power);
  CPPUNIT_TEST(test_find_root);
  CPPUNIT_TEST(test_interpolation);
  CPPUNIT_TEST_SUITE_END();

protected:
  polynomial<double> _dd1, _dd2, _dd3;
  polynomial<complex<double>> _cd1, _cd2, _cd3;
  polynomial<polynomial<double>> _rd1, _rd2, _rd3;
  polynomial<matrix2<double>> _md1, _md2, _md3;
  polynomial<AAF> _aa1, _aa2, _aa3;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test equal to. */
  void test_equal_to();

  /** Test less. */
  void test_less();

  /** Test add. */
  void test_plus();

  /** Test multiply. */
  void test_multiplies();

  /** Test power. */
  void test_power();

  /** Test find_root. */
  void test_find_root();

  /** Test interpolation */
  void test_interpolation();
};

/** @} */
#endif
