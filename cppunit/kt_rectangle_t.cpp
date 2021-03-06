#include "kt_rectangle_t.hpp"
#include <concept_test.hpp>
#include <kt_rectangle.hpp>

using namespace std;
using namespace lineda;

CPPUNIT_TEST_SUITE_REGISTRATION(kt_rectangle_TestCase);

void kt_rectangle_TestCase::setUp() {
  _r1 = new kt_rectangle(5, 6, 500, 200);
  _r2 = new kt_rectangle(point(1, 2), point(5, 6));
  _r3 = new kt_rectangle(*_r1);

  int x1 = 4, x2 = 70, y1 = 25, y2 = 90;
  point p1 = point(-35, 80), p2 = point(35, 96);
  _r4 = new kt_rectangle(x1, y1, x2, y2);
  _r5 = new kt_rectangle(p1, p2);
  _r6 = new kt_rectangle(*_r4);
}

void kt_rectangle_TestCase::tearDown() {
  delete _r1;
  delete _r2;
  delete _r3;
  delete _r4;
  delete _r5;
  delete _r6;
}

// xxxvoid kt_rectangle_TestCase::test_copy()
// xxx{
// xxx  CPPUNIT_ASSERT (copy_ok(*_r1));
// xxx  CPPUNIT_ASSERT (copy_ok(*_r2));
// xxx  CPPUNIT_ASSERT (copy_ok(*_r3));
// xxx  CPPUNIT_ASSERT (copy_ok(*_r4));
// xxx  CPPUNIT_ASSERT (copy_ok(*_r5));
// xxx  CPPUNIT_ASSERT (copy_ok(*_r6));
// xxx}

// xxxvoid kt_rectangle_TestCase::test_equal_to()  //???not pass
// xxx{
// xxx  for (size_t i=0; i<10; ++i) {
// xxx    const kt_rectangle r1 = kt_rectangle::random();
// xxx    const kt_rectangle r2 = kt_rectangle::random();
// xxx    const kt_rectangle r3 = kt_rectangle::random();
// xxx    CPPUNIT_ASSERT (equal_to_ok(r1, r2, r3));
// xxx  }
// xxx}

void kt_rectangle_TestCase::test_valid() {
  for (size_t i = 0; i < 10; ++i) {
    kt_rectangle r = kt_rectangle::random();
    CPPUNIT_ASSERT(r.is_valid());
    CPPUNIT_ASSERT(r.tlx() <= r.brx() && r.tly() <= r.bry());
  }
}

void kt_rectangle_TestCase::test_contain() {
  // xxx for (size_t i=0; i<10; ++i) {
  // xxx    kt_rectangle r1 = kt_rectangle::random();
  // xxx    kt_rectangle r2 = kt_rectangle(r1._top_left + point(5,5),
  // r1._bottom_right - point(3,4));
  // xxx    CPPUNIT_ASSERT( r2.is_fully_contained(r1) &&
  // !(r1.is_fully_contained(r2)) );
  // xxx }

  // Test for random rectangles.
  for (size_t i = 0; i < 10; ++i) {
    kt_rectangle r1 = kt_rectangle::random();
    kt_rectangle r2 = kt_rectangle::random();
    if (r1.is_fully_contained(r2)) {
      CPPUNIT_ASSERT(!(r2.is_fully_contained(r1)) || (r1 == r2));
    }
  }
}

void kt_rectangle_TestCase::test_overlap() {
  for (size_t i = 0; i < 10; ++i) {
    kt_rectangle r1 = kt_rectangle::random();
    kt_rectangle r2 = kt_rectangle::random();
    CPPUNIT_ASSERT(r1.is_overlap(r2) == r2.is_overlap(r1));
  }
}

void kt_rectangle_TestCase::test_intersect() {
  for (size_t i = 0; i < 10; ++i) {
    kt_rectangle r1 = kt_rectangle::random();
    kt_rectangle r2 = kt_rectangle::random();
    CPPUNIT_ASSERT(r1.intersect(r2) == r2.intersect(r1));
  }
}
