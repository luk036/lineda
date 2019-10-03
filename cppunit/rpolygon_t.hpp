#ifndef CPP_UNIT_RPOLYGON_HPP
#define CPP_UNIT_RPOLYGON_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <recti/rpolygon.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for rpolygon
 */
class rpolygon_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(rpolygon_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::rpolygon *s1;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();
};

/** @} */
#endif
