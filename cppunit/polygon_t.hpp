#ifndef CPP_UNIT_POLYGON_HPP
#define CPP_UNIT_POLYGON_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <polygon.hpp>

class AAF;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for polygon
 */
class polygon_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(polygon_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_area);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::polygon *_p1, *_p2, *_p3;
  recti::Polygon<AAF> *_pa1, *_pa2, *_pa3;

public:
  void setUp();
  void tearDown();

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
