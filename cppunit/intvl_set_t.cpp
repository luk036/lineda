#include "intvl_set_t.hpp"
#include <concept_test.hpp>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(intvl_set_TestCase);

using namespace std;
using namespace lineda;
using namespace cgl;
using numeric::interval;

// typedef rational<int> Rat;
// typedef interval<interval<double> > Recur;

void intvl_set_TestCase::setUp() {
  size_t size = 20;
  _id1 = new intvl_set<int>;
  interval<int> intvl;
  for (size_t i = 0; i < size; ++i) {
    intvl = interval<int>::random();
    _id1->insert(intvl);
  }

  // xxx  //double
  // xxx  _dd1 = new intvl_set<double>;
  // xxx  interval<double> intvl2;
  // xxx  for(size_t i=0; i<size; ++i) {
  // xxx    intvl2 = interval<double>::random();
  // xxx    _dd1->insert(intvl2);
  // xxx  }
  // xxx  // rational<int>
  // xxx  _cd1 = new intvl_set<Rat>;
  // xxx  interval<Rat> intvl3;
  // xxx  for(size_t i=0; i<size; ++i) {
  // xxx    intvl3 = interval<Rat>::random();
  // xxx    _cd1->insert(intvl3);
  // xxx  }
}

void intvl_set_TestCase::tearDown() {
  delete _id1; //
  // xxx  delete _dd1;
  // xxx  delete _cd1;
}

void intvl_set_TestCase::test_copy() //?
{
  CPPUNIT_ASSERT(copy_ok(_id1));
  // xxx  CPPUNIT_ASSERT (copy_ok(_cd1));
  // xxx  CPPUNIT_ASSERT (copy_ok(_dd1));
}

void intvl_set_TestCase::test_assign() //?
{
  CPPUNIT_ASSERT(assign_ok(_id1));
  // xxx  CPPUNIT_ASSERT (assign_ok(_cd1));
  // xxx  CPPUNIT_ASSERT (assign_ok(_dd1));
}

void intvl_set_TestCase::test_left_of() {
  left_of<interval<int>> left;
  for (size_t i = 0; i < 10; ++i) {
    const interval<int> i1 = interval<int>::random();
    const interval<int> i2 = interval<int>::random();
    CPPUNIT_ASSERT(left(i1, i2) == i1.left_of(i2));
  }

  // xxx  left_of<interval<double> > left2;
  // xxx  for (size_t i=0; i<10; ++i) {
  // xxx    const interval<double> i1 = interval<double>::random();
  // xxx    const interval<double> i2 = interval<double>::random();
  // xxx    CPPUNIT_ASSERT (left2(i1, i2) == i1.left_of(i2));
  // xxx  }
  // xxx
  // xxx  left_of<interval<Rat> > left3;
  // xxx  for (size_t i=0; i<10; ++i) {
  // xxx    const interval<Rat> i1 = interval<Rat>::random();
  // xxx    const interval<Rat> i2 = interval<Rat>::random();
  // xxx    CPPUNIT_ASSERT (left3(i1, i2) == i1.left_of(i2));
  // xxx  }
  // xxx
}

void intvl_set_TestCase::test_overlap() {

  for (size_t i = 0; i < 10; ++i) {
    bool over = false;
    interval<int> i1 = interval<int>::random();
    intvl_set<int> id;
    for (size_t j = 0; j < 20; ++j) {
      id.insert(interval<int>::random());
    }
    for (intvl_set<int>::iterator it = id.begin(); it != id.end(); ++it) {
      if (it->overlap(i1)) {
        over = true;
        break;
      }
    }
    CPPUNIT_ASSERT(id.overlap(i1) == over);
  }

  // xxx  for(size_t i=0; i<10; ++i) {
  // xxx    interval<double> i1 = interval<double>::random();
  // xxx      for(intvl_set<double>::iterator it=_dd1->begin(); it!=_dd1->end();
  // ++it) {      if(it->overlap(i1)) {
  // xxx	over = true;
  // xxx	break;
  // xxx      }
  // xxx      over = false;
  // xxx      }
  // xxx      CPPUNIT_ASSERT (_dd1->overlap(i1) == over);
  // xxx  }
  // xxx
  // xxx  for(size_t i=0; i<10; ++i) {
  // xxx    interval<Rat> i1 = interval<Rat>::random();
  // xxx    for(intvl_set<Rat>::iterator it=_cd1->begin(); it!=_cd1->end();
  // ++it) {      if(it->overlap(i1)) {
  // xxx      over = true;
  // xxx      break;
  // xxx    }
  // xxx    over = false;
  // xxx    }
  // xxx    CPPUNIT_ASSERT (_cd1->overlap(i1) == over);
  // xxx  }
  // xxx
}
