#include "segment_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <recti/checker.hpp>
#include <recti/rectangle.hpp>
#include <recti/segment.hpp>

using namespace std;
using namespace lineda;
using namespace recti;

CPPUNIT_TEST_SUITE_REGISTRATION(segment_TestCase);

void segment_TestCase::setUp() {
  _s1 = segment(point(1, 2), point(0, 0));
  _s2 = segment(point(-4, 5), point(3, 2));
  _s3 = segment(point(-3, -4), point(5, 5));
}

void segment_TestCase::tearDown() {}

void segment_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_s1));
  CPPUNIT_ASSERT(copy_ok(_s2));
  CPPUNIT_ASSERT(copy_ok(_s3));
}

void segment_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const segment s1 = segment::random();
    const segment s2 = segment::random();
    const segment s3 = segment::random();
    CPPUNIT_ASSERT(equal_to_ok(s1, s2, s3));
  }
}

void segment_TestCase::test_rotate() {
  for (size_t i = 0; i < 10; ++i) {
    const checker<segment> tst(segment::random());
    CPPUNIT_ASSERT(tst.rotate090_ok());
    CPPUNIT_ASSERT(tst.rotate180_ok());
    CPPUNIT_ASSERT(tst.rotate270_ok());
    CPPUNIT_ASSERT(tst.rotate_ok());
  }
}

void segment_TestCase::test_flip() {
  for (size_t i = 0; i < 10; ++i) {
    const checker<segment> tst(segment::random());
    CPPUNIT_ASSERT(tst.flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate090_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate180_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate270_then_flip_y_ok());
    CPPUNIT_ASSERT(tst.rotate_then_flip_ok());
  }
}

void segment_TestCase::test_overlap() {
  CPPUNIT_ASSERT(!_s1.overlap(_s2));
  CPPUNIT_ASSERT(_s2.overlap(_s3));
  CPPUNIT_ASSERT(!_s1.overlap(_s3));
  CPPUNIT_ASSERT(!_s2.overlap(_s1.bounding_box()));
  CPPUNIT_ASSERT(_s3.overlap(_s1.bounding_box()));
}
