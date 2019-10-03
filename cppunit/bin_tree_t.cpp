#include "bin_tree_t.hpp"
#include <concept_test.hpp>
#include <dsl/bin_tree.hpp>

using namespace lineda;
using namespace dsl;

CPPUNIT_TEST_SUITE_REGISTRATION(bin_tree_TestCase);

void bin_tree_TestCase::setUp() { s1 = new bin_tree<int>(7); }

void bin_tree_TestCase::tearDown() { delete s1; }

void bin_tree_TestCase::test_copy() {
  s1->randomize();
  CPPUNIT_ASSERT(copy_ok(*s1));
}
