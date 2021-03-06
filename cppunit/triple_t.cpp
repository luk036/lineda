#include "triple_t.hpp"
#include <concept_test.hpp>
#include <functional>
#include <triple.hpp>

using namespace boost;
using namespace numeric;

// 	$Id: triple_t.cpp 295 2006-02-16 13:49:47Z huiping $

CPPUNIT_TEST_SUITE_REGISTRATION(triple_TestCase);

using namespace std;
using namespace lineda;

typedef triple<triple<double>> Recur;

void sort(int &a, int &b, int &c) {
  if (b < a)
    std::swap(a, b);
  if (c < b)
    std::swap(b, c);
  if (b < a)
    std::swap(a, b);
}

inline triple<int> random_triple() {
  int min = (rand() - rand()) % 1000;
  int typical = (rand() - rand()) % 1000;
  int max = (rand() - rand()) % 1000;
  sort(min, typical, max);
  return triple<int>(min, typical, max);
}

void triple_TestCase::setUp() {
  _dd1 = triple<double>(1.3, 1.3, 1.3);
  _dd2 = triple<double>(4.0, 4.7, 5.0);
  _dd3 = triple<double>(2.0, 2.5, 3.0);
  // xxx _cd1 = triple<Rat>(Rat(1, 2), Rat(2, 3), Rat(2, 1));
  // xxx _cd2 = triple<Rat>(Rat(4, 5), Rat(5, 6), Rat(5, 5));
  // xxx _cd3 = triple<Rat>(Rat(2, 1), Rat(5, 2), Rat(8, 3));
  _rd1 = Recur(_dd1, _dd3, _dd2);
  _rd2 = Recur(_dd1, _dd3, _dd2);
  _rd3 = Recur(_dd1, _dd3, _dd2);
}

void triple_TestCase::tearDown() {}

void triple_TestCase::test_copy() {
  CPPUNIT_ASSERT(copy_ok(_dd1));
  // CPPUNIT_ASSERT (copy_ok(_cd1));
  CPPUNIT_ASSERT(copy_ok(_rd1));
}

void triple_TestCase::test_equal_to() {
  for (size_t i = 0; i < 10; ++i) {
    const triple<int> i1 = random_triple();
    const triple<int> i2 = random_triple();
    const triple<int> i3 = random_triple();
    CPPUNIT_ASSERT(equal_to_ok(i1, i2, i3));
  }

  CPPUNIT_ASSERT(equal_to_ok(_dd1, _dd2, _dd3));
  // CPPUNIT_ASSERT (equal_to_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(equal_to_ok(_rd1, _rd2, _rd3));
}

void triple_TestCase::test_less() {
  // xxx for (size_t i=0; i<10; ++i) {
  // xxx   const triple<int> i1 = random_triple();
  // xxx   const triple<int> i2 = random_triple();
  // xxx   const triple<int> i3 = random_triple();
  // xxx   CPPUNIT_ASSERT (less_ok(i1, i2, i3));
  // xxx }
  // xxx
  // xxx less_ok(_rd1, _rd2, _rd3);

  CPPUNIT_ASSERT(less_ok(_dd1, _dd2, _dd3));
  // CPPUNIT_ASSERT (less_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(less_ok(_rd1, _rd2, _rd3));
}

void triple_TestCase::test_plus() {
  // xxx for (size_t i=0; i<10; ++i) {
  // xxx   const triple<int> i1 = random_triple();
  // xxx   const triple<int> i2 = random_triple();
  // xxx   const triple<int> i3 = random_triple();
  // xxx   CPPUNIT_ASSERT (plus_ok(i1, i2, i3));
  // xxx }

  CPPUNIT_ASSERT(plus_ok(_dd1, _dd2, _dd3));
  // CPPUNIT_ASSERT (plus_ok(_cd1, _cd2, _cd3));
  CPPUNIT_ASSERT(plus_ok(_rd1, _rd2, _rd3));
}
