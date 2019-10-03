#ifndef CPP_UNIT_SEGMENT_HPP
#define CPP_UNIT_SEGMENT_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <segment.hpp>

/** @addtogroup cppunit
 *  @{
 */

/**
 * A test case for segment
 */
class segment_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(segment_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_equal_to);
  CPPUNIT_TEST(test_rotate);
  CPPUNIT_TEST(test_flip);
  CPPUNIT_TEST(test_overlap);
  CPPUNIT_TEST_SUITE_END();

protected:
  recti::Segment<int> _s1, _s2, _s3;

public:
  void setUp() override;
  void tearDown() override;

protected:
  /** Test copy. */
  void test_copy();

  /** Test equal to. */
  void test_equal_to();

  /** Test rotate. */
  void test_rotate();

  /** Test flip. */
  void test_flip();

  /** Test overlap. */
  void test_overlap();
};

/** @} */
#endif
