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

#ifndef CPP_UNIT_RECTANGLE_HPP
#define CPP_UNIT_RECTANGLE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "rectangle.hpp"

// P: With respect to testing, a software "class" is analogous to a
// real-world (p.159)
// P: Distributing system testing throughout the design hierarchy can
// be much more effective per testing dollar than testing at only the
// highest-level interface. (p.159)
// P: Independent testing reduces part of the risk associated with
// software integration. (p.161)
// P: Testing a component in isolation is an effective way to ensure
// reliability. (p.162)
// P: Hierarchical testing requires a separate test driver for every
// component. (p.175)
// DEF: "Incremental testing" refers to the practice of deliberately
// testing only the functionality actually implemented within the
// component under test. (p.177).
// P: Testing only the functionality *directly implemented* within a
// component enables the complexity of the test to be proportional to
// the complexity of the component. (p.178)
// P: Thorough regression testing is expensive but essential; the
// appropriate time to create thorough regression tests is tied to the
// stability of the subsystem to be tested. (p.183)

using namespace recti;

/** Test case of rectangle  */
class rectangle_t : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(rectangle_t);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_less);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_sum);
  CPPUNIT_TEST(test_area);
  CPPUNIT_TEST_SUITE_END();

protected:
  rectangle r1, r2, r3;

public:
  void setUp();
  void tearDown();

protected:
  void test_equal_to();
  void test_less();
  void test_rotate();
  void test_flip();
  void test_sum();
  void test_area();
};

/** @} */
#endif
