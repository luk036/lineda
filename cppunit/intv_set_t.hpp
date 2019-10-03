#ifndef CPP_UNIT_INTV_SET_HPP
#define CPP_UNIT_INTV_SET_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <intv_multiset.hpp>
#include <intv_set.hpp>
#include <pst_tree.hpp>
//#include <boost/rational.hpp>

/** @addtogroup cppunit
 *  @{
 */

/** A test case for intv_set */
class intv_set_TestCase : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(intv_set_TestCase);
  CPPUNIT_TEST(test_insert);
  CPPUNIT_TEST(test_erase);
  CPPUNIT_TEST(test_iterator);
  CPPUNIT_TEST_SUITE_END();

protected:
  std::intv_set<int> *_id1;         //, *_id2, *_id3, *_id4;
  std::intv_set<double> *_dd1;      //,_dd2,_dd3,;
  std::intv_multiset<int> *_id2;    //, *_id2, *_id3, *_id4;
  std::intv_multiset<double> *_dd2; //,_dd2,_dd3,;

public:
  void setUp() override;
  void tearDown() override;

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
