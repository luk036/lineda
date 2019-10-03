#include "slist_t.hpp"
#include <concept_test.hpp>
#include <dsl/slist.hpp>

using namespace lineda;
using namespace dsl;

CPPUNIT_TEST_SUITE_REGISTRATION(slist_TestCase);

void slist_TestCase::setUp() {
  s1 = new slist<int>(7); // <1, 2, 3, 4, 5, 6, 7>
}

void slist_TestCase::tearDown() { delete s1; }

void slist_TestCase::test_copy() {
  slist<int>(*s1) == *s1;
  CPPUNIT_ASSERT(copy_ok(*s1));
}

void slist_TestCase::test_reverse() { CPPUNIT_ASSERT(reverse_ok(*s1)); }
