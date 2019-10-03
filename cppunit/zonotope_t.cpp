#include "zonotope_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <recti/checker.hpp>
#include <recti/rectangle.hpp>
#include <recti/zonotope.hpp>

using namespace std;
using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(zonotope_TestCase);

void zonotope_TestCase::setUp() {
  _z1 = new zonotope(zonotope::random());
  _z2 = new zonotope(zonotope::random());
  _z3 = new zonotope(zonotope::random());
}

void zonotope_TestCase::tearDown() {
  delete _z1;
  delete _z2;
  delete _z3;
}

void zonotope_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(*_z1));
  CPPUNIT_ASSERT(copy_ok(*_z2));
  CPPUNIT_ASSERT(copy_ok(*_z3));
}

void zonotope_TestCase::test_equal_to() {
  CPPUNIT_ASSERT(equal_to_ok(*_z1, *_z2, *_z3));
}
