#include "orient_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <recti/checker.hpp>
#include <recti/orient.hpp>
#include <recti/point.hpp>
#include <recti/rectangle.hpp>

using namespace std;
using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(orient_TestCase);

void orient_TestCase::setUp() {
  _t0 = orient();
  _t1 = orient(orient::ROT180);
  _t2 = orient(orient::ROT270);
  _t3 = orient(orient::FLI090);
}

void orient_TestCase::tearDown() {}

void orient_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_t0));
  CPPUNIT_ASSERT(copy_ok(_t1));
  CPPUNIT_ASSERT(copy_ok(_t2));
  CPPUNIT_ASSERT(copy_ok(_t3));
}

void orient_TestCase::test_equal_to() {
  for (size_t i = 0; i < 64; ++i) {
    const orient t1 = orient::random();
    const orient t2 = orient::random();
    const orient t3 = orient::random();
    CPPUNIT_ASSERT(equal_to_ok(t1, t2, t3));
  }
}

void orient_TestCase::test_rotate() {
  for (size_t i = 0; i < 8; ++i) {
    const orient t1((orient::Orient)i);
    const checker<orient> tst(t1);
    CPPUNIT_ASSERT(tst.rotate090_ok());
    CPPUNIT_ASSERT(tst.rotate180_ok());
    CPPUNIT_ASSERT(tst.rotate270_ok());
    CPPUNIT_ASSERT(tst.rotate_ok());
  }
}

void orient_TestCase::test_flip() {
  for (size_t i = 0; i < 8; ++i) {
    const orient t1((orient::Orient)i);
    const checker<orient> tst(t1);
    CPPUNIT_ASSERT(tst.flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate090_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate180_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate270_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate_then_flip_ok());
  }
}

void orient_TestCase::test_xform() {
  for (size_t i = 0; i < 8; ++i) {
    const orient t1 = (orient::Orient)i;
    const orient t = _t2;
    t1.xform_from(_t2);
    t1.xform_to(_t2);
    CPPUNIT_ASSERT(_t2 == t);

    const point p1(3, 4);
    point p2 = p1;
    t1.xform_from(p2);
    t1.xform_to(p2);
    CPPUNIT_ASSERT(p1 == p2);

    const rectangle r1(p1, 5, 6);
    rectangle r2 = r1;
    t1.xform_from(r2);
    t1.xform_to(r2);
    CPPUNIT_ASSERT(r1 == r2);
  }
}
