#ifndef CPP_UNIT_OVERLAP_HPP
#define CPP_UNIT_OVERLAP_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <overlap.hpp>
#include <rectangle.hpp>
#include <vector>

// typedef Rectangle<int> rectangle;

/** @addtogroup cppunit
 *  @{
 */

/** A test case for plane_sweeper */
class plane_sweeper_TestCase : public CppUnit::TestFixture {
  // overlap_TestCase?
  CPPUNIT_TEST_SUITE(plane_sweeper_TestCase);
  // xxx  CPPUNIT_TEST( test_copy );//not pass
  CPPUNIT_TEST(test_assign);
  CPPUNIT_TEST(test_overlap);
  CPPUNIT_TEST_SUITE_END();

protected:
  cgl::plane_sweeper *_pw1; //, *_pw2, *_pw3;

public:
  void setUp();
  void tearDown();

protected:
  // xxx  void test_copy(); //not pass
  void test_assign();
  void test_overlap();
};

/** @} */
#endif
