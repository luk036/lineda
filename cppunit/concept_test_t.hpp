#ifndef CPP_UNIT_CONCEPT_TEST_HPP
#define CPP_UNIT_CONCEPT_TEST_HPP

#include <complex>
#include <concept_test.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for concept_test
 */
class concept_test_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(concept_test_TestCase);
  CPPUNIT_TEST(test_copy_ok);
  CPPUNIT_TEST(test_assign_ok);
  CPPUNIT_TEST(test_equal_to_ok);
  CPPUNIT_TEST(test_less_ok);
  CPPUNIT_TEST(test_plus_ok);
  CPPUNIT_TEST(test_multiplies_ok);
  CPPUNIT_TEST(test_ring_ok);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

protected:
  typedef std::complex<std::complex<double>> _CC;
  _CC _d1, _d2, _d3;

protected:
  /** Test copy_ok. */
  void test_copy_ok();

  /** Test assign_ok. */
  void test_assign_ok();

  /** Test equal_to_ok. */
  void test_equal_to_ok();

  /** Test less_ok. */
  void test_less_ok();

  /** Test plus_ok. */
  void test_plus_ok();

  /** Test multiply_ok. */
  void test_multiplies_ok();

  /** Test ring_ok. */
  void test_ring_ok();
};

/** @} */
#endif
