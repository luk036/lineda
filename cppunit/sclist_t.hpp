#ifndef CPP_UNIT_SCLIST_HPP
#define CPP_UNIT_SCLIST_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <dsl/sclist.hpp>

/* Forward declaration */
// xxx template <typename _Tp> class sclist;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for sclist
 */
class sclist_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(sclist_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_reverse);
  CPPUNIT_TEST(test_josephus_permutate);
  CPPUNIT_TEST_SUITE_END();

protected:
  dsl::sclist<int> *s1;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test reverse. */
  void test_reverse();

  /** Test Josephus Permutate. */
  void test_josephus_permutate();
};

/** @} */
#endif
