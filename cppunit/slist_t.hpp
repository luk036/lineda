#ifndef CPP_UNIT_SLIST_HPP
#define CPP_UNIT_SLIST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <dsl/slist.hpp>

/* Forward declaration */
// xxx template <typename _Tp> class slist;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for slist
 */
class slist_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(slist_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_reverse);
  CPPUNIT_TEST_SUITE_END();

protected:
  dsl::slist<int> *s1;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test reverse. */
  void test_reverse();
};

/** @} */
#endif
