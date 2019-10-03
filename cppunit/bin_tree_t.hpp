#ifndef CPP_UNIT_BIN_TREE_HPP
#define CPP_UNIT_BIN_TREE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <dsl/bin_tree.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for bin_tree
 */
class bin_tree_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(bin_tree_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST_SUITE_END();

protected:
  dsl::bin_tree<int> *s1;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();
};

/** @} */
#endif
