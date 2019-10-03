#include "intv_set_t.hpp"
#include <concept_test.hpp>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(intv_set_TestCase);

using namespace std;
using namespace lineda;

void intv_set_TestCase::setUp() {
  int s[] = {33, 32, 2, 5, 3, 6, 9, 3, 4, 45, 43, 76, 23, 11};
  int t[] = {3, 2, 2, 5, 33, 36, 29, 33, 4, 45, 43, 6, 3, 1};
  size_t N = sizeof(s) / sizeof(int);

  _id1 = new intv_set<int>(s, s + N, t);
  _dd1 = new intv_set<double>(s, s + N, t);
  _id2 = new intv_multiset<int>(s, s + N, t);
  _dd2 = new intv_multiset<double>(s, s + N, t);
}

void intv_set_TestCase::tearDown() {
  delete _id1;
  delete _dd1;
  delete _id2;
  delete _dd2;
}

void intv_set_TestCase::test_insert() {
  _id1->insert(45, 7);
  _dd1->insert(45, 7);
  _id2->insert(45, 7);
  _dd2->insert(45, 7);
}

void intv_set_TestCase::test_erase() {
  _id1->erase(45, 7);
  _dd1->erase(45, 7);
  _id2->erase(45, 7);
  _dd2->erase(45, 7);
}

void intv_set_TestCase::test_iterator() {
  intv_set<int>::iterator it1 = _id1->lower_bound(4, 75);
  intv_set<double>::iterator it2 = _dd1->lower_bound(4, 75);
  intv_multiset<int>::iterator it3 = _id2->lower_bound(4, 75);
  intv_multiset<double>::iterator it4 = _dd2->lower_bound(4, 75);

  for (; it1 != _id1->end(); ++it1)
    ;
  for (; it2 != _dd1->end(); ++it2)
    ;
  for (; it3 != _id2->end(); ++it3)
    ;
  for (; it4 != _dd2->end(); ++it4)
    ;
}
