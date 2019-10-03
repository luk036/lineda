#ifndef CPP_UNIT_INTVL_SET_HPP
#define CPP_UNIT_INTVL_SET_HPP

#include <boost/rational.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <intvl_set.hpp>

// using namespace cgl;
// using namespace numeric;

typedef boost::rational<int> Rat; // xxx necessary?

/** @addtogroup cppunit
 *  @{
 */

/** A test case for intvl_set */
class intvl_set_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(intvl_set_TestCase);
  CPPUNIT_TEST(test_copy);
  CPPUNIT_TEST(test_assign);
  CPPUNIT_TEST(test_left_of);
  CPPUNIT_TEST(test_overlap);
  CPPUNIT_TEST_SUITE_END();

protected:
  cgl::intvl_set<int> *_id1;                  //, *_id2, *_id3, *_id4;
  cgl::intvl_set<double> *_dd1;               //,_dd2,_dd3,;
  cgl::intvl_set<boost::rational<int>> *_cd1; //, _cd2, _cd3;

public:
  void setUp();
  void tearDown();

protected:
  /** Test copy. */
  void test_copy();

  /** Test assign. */
  void test_assign();

  /** Test left of. */
  void test_left_of();

  /** Test overlap. */
  void test_overlap();
};

/** @} */
#endif
