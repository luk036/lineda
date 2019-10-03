#ifndef CPP_UNIT_INTERVAL_HPP
#define CPP_UNIT_INTERVAL_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <interval.hpp>
// xxx #include <boost/rational.hpp>

/**
 * @defgroup cppunit Unit Test Module
 * @ingroup lineda
 * This module provides individual testing for all other modules
 * below. Ideally each class should have at least one test suit.
 * Use data with known properties to verify implementation. E.g.
 * i) Use a matrix with known eigenvalues to verify your conjugate
 * gradient code (using the corresponding convergence theorems).
 * ii) Use random genrated data to verify invariant properties
 * (e.g. reflexivity, symmetry, transitivity, rational symmetric,
 * commutative, associative, convexity, monotonity, orthogonality)
 * iii) Cover all corner cases as possible.
 */

/** @addtogroup cppunit
 *  @{
 */

// typedef boost::rational<int> Rat;

/**
 * A test case for interval.
 */
class interval_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(interval_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_assign);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_less);
  CPPUNIT_TEST(test_plus);
  CPPUNIT_TEST(test_multiplies);
  CPPUNIT_TEST(test_intersect);
  CPPUNIT_TEST_SUITE_END();

protected:
  numeric::interval<double> _dd1, _dd2, _dd3;
  // xxx numeric::interval<boost::rational<int> >      _cd1, _cd2, _cd3;
  numeric::interval<numeric::interval<double>> _rd1, _rd2, _rd3;

public:
  void setUp();
  void tearDown();

protected:
  /** Test copy. */
  void test_copy();

  /** Test assign. */
  void test_assign();

  /** Test equal to. */
  void test_equal_to();

  /** Test less. */
  void test_less();

  /** Test plus. */
  void test_plus();

  /** Test multiplies. */
  void test_multiplies();

  /** Test intersect */
  void test_intersect();
};

/** @} */
#endif
