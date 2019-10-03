#include "polygon_t.hpp"
#include <aa_aaf.h>
#include <concept_test.hpp>
#include <functional>
#include <recti/checker.hpp>
#include <recti/polygon.hpp>
#include <recti/rectangle.hpp>

using namespace std;
using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(polygon_TestCase);

void polygon_TestCase::setUp() {
  point apt1[] = {point(3, 6), point(7, 6), point(7, 9),
                  point(2, 9), point(2, 8), point(3, 8)};
  rectangle r(point(4, 5), 5, 6);

  Point<AAF> lower_left_p =
      Point<AAF>(AAF(interval(2, 5)), AAF(interval(2, 5)));
  AAF width = AAF(interval(3, 4));
  AAF height = AAF(interval(4, 5));
  Rectangle<AAF> ra(lower_left_p, width, height);

  // xxx _p1 = new polygon(apt1, 6);
  _p1 = new polygon(r);
  _p2 = new polygon(r);
  // xxx _p3 = new polygon(apt1, 4);
  _p3 = new polygon(r);

  _pa1 = new Polygon<AAF>(ra);
  _pa2 = new Polygon<AAF>(ra);
  _pa3 = new Polygon<AAF>(ra);
}

void polygon_TestCase::tearDown() {
  delete _p1;
  delete _p2;
  delete _p3;

  delete _pa1;
  delete _pa2;
  delete _pa3;
}

void polygon_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(*_p1));
  CPPUNIT_ASSERT(copy_ok(*_p2));
  CPPUNIT_ASSERT(copy_ok(*_p3));

  CPPUNIT_ASSERT(copy_ok(*_pa1));
  CPPUNIT_ASSERT(copy_ok(*_pa2));
  CPPUNIT_ASSERT(copy_ok(*_pa3));
}

void polygon_TestCase::test_equal_to() {
  CPPUNIT_ASSERT(equal_to_ok(*_p1, *_p2, *_p3));
  CPPUNIT_ASSERT(equal_to_ok(*_pa1, *_pa2, *_pa3));
}

void polygon_TestCase::test_rotate() {
  const checker<polygon> tst1(*_p1);
  CPPUNIT_ASSERT(tst1.rotate090_ok());
  CPPUNIT_ASSERT(tst1.rotate180_ok());
  CPPUNIT_ASSERT(tst1.rotate270_ok());
  CPPUNIT_ASSERT(tst1.rotate_ok());

  const checker<polygon> tst2(*_p2);
  CPPUNIT_ASSERT(tst2.rotate090_ok());
  CPPUNIT_ASSERT(tst2.rotate180_ok());
  CPPUNIT_ASSERT(tst2.rotate270_ok());
  CPPUNIT_ASSERT(tst2.rotate_ok());

  const checker<polygon> tst3(*_p3);
  CPPUNIT_ASSERT(tst3.rotate090_ok());
  CPPUNIT_ASSERT(tst3.rotate180_ok());
  CPPUNIT_ASSERT(tst3.rotate270_ok());
  CPPUNIT_ASSERT(tst3.rotate_ok());

  const checker<Polygon<AAF>> tst1(*_pa1);
  CPPUNIT_ASSERT(tst1.rotate090_ok());
  CPPUNIT_ASSERT(tst1.rotate180_ok());
  CPPUNIT_ASSERT(tst1.rotate270_ok());
  CPPUNIT_ASSERT(tst1.rotate_ok());

  const checker<Polygon<AAF>> tst2(*_pa2);
  CPPUNIT_ASSERT(tst2.rotate090_ok());
  CPPUNIT_ASSERT(tst2.rotate180_ok());
  CPPUNIT_ASSERT(tst2.rotate270_ok());
  CPPUNIT_ASSERT(tst2.rotate_ok());

  const checker<Polygon<AAF>> tst3(*_pa3);
  CPPUNIT_ASSERT(tst3.rotate090_ok());
  CPPUNIT_ASSERT(tst3.rotate180_ok());
  CPPUNIT_ASSERT(tst3.rotate270_ok());
  CPPUNIT_ASSERT(tst3.rotate_ok());
}

void polygon_TestCase::test_flip() {
  const checker<polygon> tst1(*_p1);
  CPPUNIT_ASSERT(tst1.flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate_then_flip_ok());

  const checker<polygon> tst2(*_p2);
  CPPUNIT_ASSERT(tst2.flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate_then_flip_ok());

  const checker<polygon> tst3(*_p3);
  CPPUNIT_ASSERT(tst3.flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate_then_flip_ok());

  const checker<Polygon<AAF>> tst1(*_pa1);
  CPPUNIT_ASSERT(tst1.flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst1.rotate_then_flip_ok());

  const checker<Polygon<AAF>> tst2(*_pa2);
  CPPUNIT_ASSERT(tst2.flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst2.rotate_then_flip_ok());

  const checker<Polygon<AAF>> tst3(*_pa3);
  CPPUNIT_ASSERT(tst3.flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate090_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate180_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate270_then_flip_y_ok());
  CPPUNIT_ASSERT(tst3.rotate_then_flip_ok());
}

void polygon_TestCase::test_area() {
  CPPUNIT_ASSERT_EQUAL(30, _p1->area());
  CPPUNIT_ASSERT_EQUAL(30, _p2->area());
  CPPUNIT_ASSERT_EQUAL(30, _p3->area());

  CPPUNIT_ASSERT_EQUAL(30, _pa1->area());
  CPPUNIT_ASSERT_EQUAL(30, _pa2->area());
  CPPUNIT_ASSERT_EQUAL(30, _pa3->area());
}
