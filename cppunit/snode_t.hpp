#ifndef CPP_UNIT_SNODE_HPP
#define CPP_UNIT_SNODE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <dsl/snode.hpp>

/* Forward declaration */
// xxx template <typename _Tp> class snode;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for snode
 */
class snode_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(snode_TestCase);
  CPPUNIT_TEST(test_reverse);
  CPPUNIT_TEST(test_josephus_permutate);
  CPPUNIT_TEST_SUITE_END();

protected:
  dsl::snode_base s1, s2, s3, s4, s5, s6, s7;

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
