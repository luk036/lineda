#include "path_t.hpp"
#include <concept_test.hpp>
#include <recti/path.hpp>
#include <recti/rpolygon.hpp>

using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(path_TestCase);

void path_TestCase::setUp() { s1 = new path(path::random(8)); }

void path_TestCase::tearDown() { delete s1; }

void path_TestCase::test_copy() { CPPUNIT_ASSERT(copy_ok(*s1)); }

void path_TestCase::test_create_outline() {
  // xxx rpolygon* rp = s1->create_outline();
  // xxx rp->normalize();
  // xxx CPPUNIT_ASSERT_EQUAL (rp->area(), s1->area());
  // xxx delete rp;
}
