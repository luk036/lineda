#include "rpolygon_t.hpp"
#include <concept_test.hpp>
#include <recti/rpolygon.hpp>

using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(rpolygon_TestCase);

void rpolygon_TestCase::setUp() { s1 = new rpolygon(rpolygon::random(100)); }

void rpolygon_TestCase::tearDown() { delete s1; }

void rpolygon_TestCase::test_copy() { CPPUNIT_ASSERT(copy_ok(*s1)); }
