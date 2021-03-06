#include "point_t.hpp"
#include <aa_aaf.h>
#include <concept_test.hpp>
#include <functional>
#include <iostream>
#include <numeric/interval.hpp>
#include <recti/checker.hpp>
#include <recti/point.hpp>

using namespace std;
using namespace lineda;
// xxx using namespace numeric;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(point_TestCase);

void point_TestCase::setUp() {
  _p0 = new point(0, 0);
  _p1 = new point(1, 2);
  _p2 = new point(4, 5);
  _p3 = new point(2, 1);

  _pa0 = new Point<AAF>(AAF(interval(3.0, 6.0)), AAF(interval(4.0, 5.0)));
  _pa1 = new Point<AAF>(AAF(interval(4.0, 5.0)), AAF(interval(5.0, 8.0)));
  _pa2 = new Point<AAF>(AAF(interval(1.0, 2.0)), AAF(interval(1.0, 9.0)));
  _pa3 = new Point<AAF>(AAF(interval(3.0, 4.0)), AAF(interval(2.0, 8.0)));
}

void point_TestCase::tearDown() {
  delete _p0;
  delete _p1;
  delete _p2;
  delete _p3;

  delete _pa0;
  delete _pa1;
  delete _pa2;
  delete _pa3;
}

void point_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(*_p0));
  CPPUNIT_ASSERT(copy_ok(*_p1));
  CPPUNIT_ASSERT(copy_ok(*_p2));
  CPPUNIT_ASSERT(copy_ok(*_p3));

  CPPUNIT_ASSERT(copy_ok(*_pa0));
  CPPUNIT_ASSERT(copy_ok(*_pa1));
  CPPUNIT_ASSERT(copy_ok(*_pa2));
  CPPUNIT_ASSERT(copy_ok(*_pa3));
}

void point_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const point p1 = point::random();
    const point p2 = point::random();
    const point p3 = point::random();
    CPPUNIT_ASSERT(equal_to_ok(p1, p2, p3));
  }

  for (size_t i = 0; i < 10; ++i) {
    AAF x = AAF(interval(rand() % 10, rand() % 5));
    AAF y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa1 = Point<AAF>(x, y);

    x = AAF(interval(rand() % 10, rand() % 5));
    y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa2 = Point<AAF>(x, y);

    x = AAF(interval(rand() % 10, rand() % 5));
    y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa3 = Point<AAF>(x, y);
    CPPUNIT_ASSERT(equal_to_ok(pa1, pa2, pa3));
  }
}

void point_TestCase::test_less() {
  for (size_t i = 0; i < 10; ++i) {
    const point p1 = point::random();
    const point p2 = point::random();
    const point p3 = point::random();
    CPPUNIT_ASSERT(less_ok(p1, p2, p3));
  }

  // xxx for (size_t i=0; i<10; ++i) {
  // xxx   AAF x=AAF(interval(rand()%10,rand()%5));
  // xxx   AAF y=AAF(interval(rand()%3,rand()%7));
  // xxx   const Point<AAF> pa1 = Point<AAF>(x,y);

  // xxx   x=AAF(interval(rand()%10,rand()%5));
  // xxx   y=AAF(interval(rand()%3,rand()%7));
  // xxx   const Point<AAF> pa2 = Point<AAF>(x,y);

  // xxx   x=AAF(interval(rand()%10,rand()%5));
  // xxx   y=AAF(interval(rand()%3,rand()%7));
  // xxx const Point<AAF> pa3 = Point<AAF>(x,y);
  // xxx CPPUNIT_ASSERT (less_ok(pa1, pa2, pa3));
  // xxx }
}

void point_TestCase::test_sum() {
  for (size_t i = 0; i < 10; ++i) {
    AAF x = AAF(interval(rand() % 10, rand() % 5));
    AAF y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa1 = Point<AAF>(x, y);

    x = AAF(interval(rand() % 10, rand() % 5));
    y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa2 = Point<AAF>(x, y);

    x = AAF(interval(rand() % 10, rand() % 5));
    y = AAF(interval(rand() % 3, rand() % 7));
    const Point<AAF> pa3 = Point<AAF>(x, y);
    CPPUNIT_ASSERT(plus_ok(pa1, pa2, pa3));
  }
}

void point_TestCase::test_rotate() {
  for (size_t i = 0; i < 10; ++i) {
    const checker<point> tst(point::random());
    CPPUNIT_ASSERT(tst.rotate090_ok());
    CPPUNIT_ASSERT(tst.rotate180_ok());
    CPPUNIT_ASSERT(tst.rotate270_ok());
    CPPUNIT_ASSERT(tst.rotate_ok());
  }

  for (size_t i = 0; i < 10; ++i) {
    AAF x = AAF(interval(rand() % 10, rand() % 5));
    AAF y = AAF(interval(rand() % 3, rand() % 7));
    Point<AAF> p = Point<AAF>(x, y);
    const checker<Point<AAF>> tst(p);
    CPPUNIT_ASSERT(tst.rotate090_ok());
    CPPUNIT_ASSERT(tst.rotate180_ok());
    CPPUNIT_ASSERT(tst.rotate270_ok());
    CPPUNIT_ASSERT(tst.rotate_ok());
  }
}

void point_TestCase::test_flip() {
  for (size_t i = 0; i < 10; ++i) {
    const checker<point> tst(point::random());
    CPPUNIT_ASSERT(tst.flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate090_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate180_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate270_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate_then_flip_ok());
  }

  for (size_t i = 0; i < 10; ++i) {
    AAF x = AAF(interval(rand() % 10, rand() % 5));
    AAF y = AAF(interval(rand() % 3, rand() % 7));
    Point<AAF> p = Point<AAF>(x, y);
    const checker<Point<AAF>> tst(p);
    CPPUNIT_ASSERT(tst.flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate090_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate180_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate270_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate_then_flip_ok());
  }
}
