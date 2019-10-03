#ifndef CPP_UNIT_PATH_HPP
#define CPP_UNIT_PATH_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <recti/path.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for path
 */
class path_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(path_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_create_outline);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::path *s1;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test create_outline */
  void test_create_outline();
};

/** @} */
#endif
