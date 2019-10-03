#ifndef CPP_UNIT_ZONOTOPE_HPP
#define CPP_UNIT_ZONOTOPE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <zonotope.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for zonotope
 */
class zonotope_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(zonotope_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::zonotope *_z1, *_z2, *_z3;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test equal to. */
  void test_equal_to();

  /** Test rotate. */
  void test_rotate();

  /** Test flip. */
  void test_flip();

  /** Test area. */
  void test_area();
};

/** @} */
#endif
