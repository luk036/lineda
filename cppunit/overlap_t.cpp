#include "overlap_t.hpp"
#include <concept_test.hpp>
#include <recti/rectangle.hpp>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(plane_sweeper_TestCase);

using namespace std;
using namespace lineda;
using namespace cgl;
using namespace recti;

void plane_sweeper_TestCase::setUp() {
  size_t size = 60;

  vector<rectangle> irects(size);
  for (size_t i = 0; i < size; ++i) {
    irects.push_back(rectangle::random()); // Rectangle<int>::random();
  }
  //_pw1 = new plane_sweeper;
  //*_pw1 = plane_sweeper(irects);
  _pw1 = new plane_sweeper(irects);

  // xxx  // double
  // xxx  vector<Rectangle<double>> d_rects(size);
  // xxx  for (size_t i=0; i<size; ++i) {
  // xxx    d_rects[i] = Point<interval<double>>::random();
  // xxx  }
  // xxx  pw2 = new plane_sweeper(d_rects);
  // xxx
  // xxx  //interval<int>
  // xxx  vector<rectangle> v_rects(size);
  // xxx  for (size_t i=0; i<size; ++i) {
  // xxx    v_rects[i] = Point<interval<interval<int>>>::random();
  // xxx  }
  // xxx  pw3 = new plane_sweeper(_irects);
  // xxx
  // xxx  CPPUNIT_ASSERT (equal_to_ok(_dd1, _dd2, _dd3));
}

void plane_sweeper_TestCase::tearDown() {
  delete _pw1;
  // xxx  delete _pw2;
  // xxx  delete _pw3;
}

// xxxvoid plane_sweeper_TestCase::test_copy() //compilation not passed
// xxx{
// xxx  CPPUNIT_ASSERT (copy_ok(*_pw1));
// xxx}

void plane_sweeper_TestCase::test_assign() { CPPUNIT_ASSERT(assign_ok(_pw1)); }

void plane_sweeper_TestCase::test_overlap() {
  /** Test for random-generated rectangles. */
  for (size_t i = 0; i < 10; ++i) {
    size_t size = 20;
    vector<rectangle> rects(size);
    for (size_t i = 0; i < size; ++i) {
      rects.push_back(rectangle::random()); // Rectangle<int>::random();
    }
    plane_sweeper pw(rects);

    bool over = false;
    for (size_t i = 0; i < rects.size(); ++i) {
      for (size_t j = i + 1; j < rects.size(); ++j) {
        if (rects[i].overlap(rects[j])) {
          over = true;
          break;
        }
      }
      if (over) {
        break;
      }
    }
    CPPUNIT_ASSERT(pw.overlap() == over);
  }

  /** Test for pre-assigned rectangles. : not overlap */
  rectangle r1 =
      rectangle(point(-5, -50), 10,
                20); //<interval<int>(-5 ,20), interval<int>(-50, 40)>;
  Point<int> p1 = point(20, 30);
  size_t size = 60;
  vector<rectangle> ps_rects(size); // preser rectangles
  ps_rects[0] = r1;
  for (size_t i = 1; i < size; ++i) {
    ps_rects[i] = r1 + p1;
  }
  plane_sweeper pw2(ps_rects);
  bool over = false;
  for (size_t i = 0; i < ps_rects.size(); ++i) {
    for (size_t j = i + 1; j < ps_rects.size(); ++j) {
      if (ps_rects[i].overlap(ps_rects[j])) {
        over = true;
        break;
      }
    }
    if (over) {
      break;
    }
  }
  CPPUNIT_ASSERT(pw2.overlap() == over);

  /** Test for pre-assigned rectangles:overlap! */
  Point<int> p2 = point(30, 40);
  ps_rects[10] = r1 + p2;
  plane_sweeper pw3(ps_rects);
  over = false;
  for (size_t i = 0; i < ps_rects.size(); ++i) {
    for (size_t j = i + 1; j < ps_rects.size(); ++j) {
      if (ps_rects[i].overlap(ps_rects[j])) {
        over = true;
        break;
      }
    }
    if (over) {
      break;
    }
  }
  CPPUNIT_ASSERT(pw3.overlap() == over);
}
