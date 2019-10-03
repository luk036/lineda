#ifndef CPP_UNIT_ORIENT_HPP
#define CPP_UNIT_ORIENT_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <orient.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for orient
 */
class orient_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(orient_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_xform);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::orient _t0, _t1, _t2, _t3;

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

  /** Test transformation. */
  void test_xform();
};

/** @} */
#endif
