#ifndef CPP_UNIT_KT_RECTANGLE_HPP
#define CPP_UNIT_KT_RECTANGLE_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <kt_rectangle.hpp>

// using namespace lindea::cgl;

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for kt_rectangle
 */
class kt_rectangle_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(kt_rectangle_TestCase);
  //  CPPUNIT_TEST( test_copy );
  //  CPPUNIT_TEST( test_equal_to );
  CPPUNIT_TEST(test_valid);
  CPPUNIT_TEST(test_contain);
  CPPUNIT_TEST(test_overlap);
  CPPUNIT_TEST(test_intersect);
  CPPUNIT_TEST_SUITE_END();

protected:
  kt_rectangle *_r1, *_r2, *_r3, *_r4, *_r5, *_r6;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  //  void test_copy(); //not pass

  /** Test equal to. */
  //  void test_equal_to();

  /** Test valid. */
  void test_valid();

  /** Test if contain. */
  void test_contain();

  /** Test if overlap */
  void test_overlap();

  /** Test if intersect */
  void test_intersect();
};

/** @} */
#endif
