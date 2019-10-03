#include "intv_map_t.hpp"
#include <concept_test.hpp>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(intv_map_TestCase);

using namespace std;
using namespace lineda;

void intv_map_TestCase::setUp() {
  _id1 = new intv_map<int, double>();
  _dd1 = new intv_map<double, int>();
  _id2 = new intv_multimap<int, double>();
  _dd2 = new intv_multimap<double, int>();
}

void intv_map_TestCase::tearDown() {
  delete _id1;
  delete _dd1;
  delete _id2;
  delete _dd2;
}

void intv_map_TestCase::test_insert() {
  _id1->insert(make_pair(45, 5.5), 7);
  _dd1->insert(make_pair(5.5, 4), 7.3);
  _id2->insert(make_pair(45, 5.5), 7);
  _dd2->insert(make_pair(5.5, 4), 7.3);
}

void intv_map_TestCase::test_erase() {
  _id1->erase(45, 7);
  _dd1->erase(5.5, 7.3);
  _id2->erase(45, 7);
  _dd2->erase(5.5, 7.3);
}

void intv_map_TestCase::test_iterator() {
  intv_map<int, double>::iterator it1 = _id1->lower_bound(4, 75);
  intv_map<double, int>::iterator it2 = _dd1->lower_bound(4, 75);
  intv_multimap<int, double>::iterator it3 = _id2->lower_bound(4, 75);
  intv_multimap<double, int>::iterator it4 = _dd2->lower_bound(4, 75);

  for (; it1 != _id1->end(); ++it1)
    ;
  for (; it2 != _dd1->end(); ++it2)
    ;
  for (; it3 != _id2->end(); ++it3)
    ;
  for (; it4 != _dd2->end(); ++it4)
    ;
}
