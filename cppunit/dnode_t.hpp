#ifndef CPP_UNIT_DNODE_HPP
#define CPP_UNIT_DNODE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <dsl/dnode.hpp>

/* Forward declaration */
// xxx template <typename _Tp> class dnode;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for dnode
 */
class dnode_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(dnode_TestCase);
  CPPUNIT_TEST(test_reverse);
  CPPUNIT_TEST(test_josephus_permutate);
  CPPUNIT_TEST_SUITE_END();

protected:
  dsl::dnode_base s1, s2, s3, s4, s5, s6, s7;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test reverse. */
  void test_reverse();

  /** Test Josephus Permutate. */
  void test_josephus_permutate();
};

/** @} */
#endif
