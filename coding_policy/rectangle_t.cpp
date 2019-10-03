// Sample header file to illustrate the coding policy. The rules are
// based on the books "Large-Scale C++ Software Design" and "Effective
// C++, 2nd Ed"
//
//  DEF - definition
//  MDR - major design rule
//  mdr - minor design rule
//  GL  - guideline
//  P   - principle
//
//  effc++ - rules from Effective C++.
//

#include "rectangle_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <geometry_test.hpp>
#include <rectangle.hpp>

using namespace std;
using namespace lineda;

CPPUNIT_TEST_SUITE_REGISTRATION(rectangle_t);

void rectangle_t::setUp() {
  r1 = rectangle(point(1, 2), 0, 0);
  r2 = rectangle(point(-4, 5), 3, 2);
  r3 = rectangle(point(-3, -4), 5, 5);
}

void rectangle_t::tearDown() {}

void rectangle_t::test_equal_to() {
  for (size_t i = 0; i < 100; ++i) {
    const rectangle r1 = rectangle::random();
    const rectangle r2 = rectangle::random();
    const rectangle r3 = rectangle::random();
    CPPUNIT_ASSERT(equivalent_ok(r1, r2, r3, equal_to<rectangle>()));
  }
}

void rectangle_t::test_area() {
  CPPUNIT_ASSERT_EQUAL(0., r1.area());
  CPPUNIT_ASSERT_EQUAL(6., r2.area());
  CPPUNIT_ASSERT_EQUAL(25., r3.area());
}
