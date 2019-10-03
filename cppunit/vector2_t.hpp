#ifndef CPP_UNIT_VECTOR2_HPP
#define CPP_UNIT_VECTOR2_HPP

#include <complex>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector2.hpp>

using std::complex;
using numeric::vector2;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for vector2
 */
class vector2_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(vector2_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_assign);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_plus);
  CPPUNIT_TEST(test_multiplies);
  CPPUNIT_TEST(test_power);
  CPPUNIT_TEST_SUITE_END();

protected:
  numeric::vector2<double> _dd1, _dd2, _dd3;
  numeric::vector2<std::complex<double>> _cd1, _cd2, _cd3;
  numeric::vector2<numeric::vector2<double>> _rd1, _rd2, _rd3;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test assign. */
  void test_assign();

  /** Test equal to. */
  void test_equal_to();

  /** Test add. */
  void test_plus();

  /** Test multiply. */
  void test_multiplies();

  /** Test power. */
  void test_power();
};

/** @} */
#endif
