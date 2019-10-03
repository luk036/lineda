#ifndef CPP_UNIT_INTV_MAP_HPP
#define CPP_UNIT_INTV_MAP_HPP

#include <boost/rational.hpp>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <intv_map.hpp>
#include <intv_multimap.hpp>
#include <pst_tree.hpp>

// using namespace cgl;
// using namespace numeric;

typedef boost::rational<int> Rat; // xxx necessary?

/** @addtogroup cppunit
 *  @{
 */

/** A test case for intv_map */
class intv_map_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(intv_map_TestCase);
  CPPUNIT_TEST(test_insert);
  CPPUNIT_TEST(test_erase);
  CPPUNIT_TEST(test_iterator);
  CPPUNIT_TEST_SUITE_END();

protected:
  std::intv_map<int, double> *_id1;      //, *_id2, *_id3, *_id4;
  std::intv_map<double, int> *_dd1;      //,_dd2,_dd3,;
  std::intv_multimap<int, double> *_id2; //, *_id2, *_id3, *_id4;
  std::intv_multimap<double, int> *_dd2; //,_dd2,_dd3,;

public:
  void setUp();
  void tearDown();

protected:
  /** Test insert. */
  void test_insert();

  /** Test erase. */
  void test_erase();

  /** Test iterator. */
  void test_iterator();
};

/** @} */
#endif
