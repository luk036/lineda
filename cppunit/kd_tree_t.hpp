#ifndef CPP_UNIT_KDTREE_HPP
#define CPP_UNIT_KDTREE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <kd_tree.hpp>

// using namespace cgl;
// using namespace lineda;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for kd-tree
 */
class kd_tree_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(kd_tree_TestCase);
  // xxx  CPPUNIT_TEST( test_copy );
  // xxx  CPPUNIT_TEST( test_equal_to );
  CPPUNIT_TEST(test_create);
  CPPUNIT_TEST(test_search);
  CPPUNIT_TEST_SUITE_END();

protected:
  kd_tree *_t1, *_t2, *_t3;

public:
  void setUp() override;
  void tearDown() override;

protected:
  // xxx /** Test copy. */
  // xxx void test_copy();
  // xxx
  // xxx /** Test equal to. */
  // xxx void test_equal_to();

  /** Test create. */
  void test_create();

  /** Test search. */
  void test_search();
};

/** @} */
#endif
