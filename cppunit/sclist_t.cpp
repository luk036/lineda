#include "sclist_t.hpp"
#include <concept_test.hpp>
#include <dsl/sclist.hpp>

using namespace lineda;
using namespace dsl;

CPPUNIT_TEST_SUITE_REGISTRATION(sclist_TestCase);

void sclist_TestCase::setUp() {
  s1 = new sclist<int>(7); // <1, 2, 3, 4, 5, 6, 7>
}

void sclist_TestCase::tearDown() { delete s1; }

void sclist_TestCase::test_copy() { CPPUNIT_ASSERT(copy_ok(*s1)); }

void sclist_TestCase::test_reverse() { CPPUNIT_ASSERT(reverse_ok(*s1)); }

void sclist_TestCase::test_josephus_permutate() {
  sclist<int> s2(*s1);
  s2.josephus_permutate(3); // <3, 6, 2, 7, 5, 1, 4>
  s2.josephus_permutate(3); // <2, 1, 6, 4, 5, 3, 7>
  s2.josephus_permutate(3); // <6, 3, 1, 7, 5, 2, 4>
  s2.josephus_permutate(3); // <1, 2, 3, 4, 5, 6, 7>
  CPPUNIT_ASSERT(s2 == *s1);
}
