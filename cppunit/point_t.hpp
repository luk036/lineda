#ifndef CPP_UNIT_POINT_HPP
#define CPP_UNIT_POINT_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <point.hpp>

/** @addtogroup cppunit
 *  @{
 */

class AAF;

/**
 * A test case for point
 */
class point_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(point_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_less);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_sum);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::point *_p0, *_p1, *_p2, *_p3;
  recti::Point<AAF> *_pa0, *_pa1, *_pa2, *_pa3;

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
};

/** @} */
#endif
