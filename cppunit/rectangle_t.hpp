#ifndef CPP_UNIT_RECTANGLE_HPP
#define CPP_UNIT_RECTANGLE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <rectangle.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for rectangle
 */
class rectangle_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(rectangle_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_less);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_sum);
  CPPUNIT_TEST(test_area);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::Rectangle<int> _r1, _r2, _r3;

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

  /** Test rotate. */
  void test_rotate();

  /** Test flip. */
  void test_flip();

  /** Test Minsowki sum */
  void test_sum();

  /** Test area. */
  void test_area();
};

/** @} */
#endif
