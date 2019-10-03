#include "snode_t.hpp"
#include <concept_test.hpp>
#include <dsl/snode.hpp>

using namespace lineda;
using namespace dsl;

CPPUNIT_TEST_SUITE_REGISTRATION(snode_TestCase);

void snode_TestCase::setUp() {}

void snode_TestCase::tearDown() {}

void snode_TestCase::test_reverse() {
  s1._next = &s2;
  s2._next = &s3;
  s3._next = &s4;
  s4._next = &s5;
  s5._next = &s6;
  s6._next = &s7;
  s7._next = nullptr;
  CPPUNIT_ASSERT_EQUAL(&s7, s1.reverse());
  CPPUNIT_ASSERT_EQUAL((snode_base *)nullptr, s1._next);
  CPPUNIT_ASSERT_EQUAL(&s1, s7.reverse());
  CPPUNIT_ASSERT_EQUAL((snode_base *)nullptr, s7._next);

  s7._next = &s1;
  CPPUNIT_ASSERT_EQUAL(&s1, s1.reverse());
  CPPUNIT_ASSERT_EQUAL(&s7, s1._next);
  CPPUNIT_ASSERT_EQUAL(&s1, s1.reverse());
  CPPUNIT_ASSERT(s1.loop_back());

  s7._next = &s4;
  CPPUNIT_ASSERT_EQUAL(&s1, s1.reverse());
  CPPUNIT_ASSERT_EQUAL(&s7, s4._next);
  CPPUNIT_ASSERT_EQUAL(&s1, s1.reverse());
  CPPUNIT_ASSERT(s1.has_cycle());
}

void snode_TestCase::test_josephus_permutate() {
  s1._next = &s2;
  s2._next = &s3;
  s3._next = &s4;
  s4._next = &s5;
  s5._next = &s6;
  s6._next = &s7;
  s7._next = &s1;
  snode_base *s = &s1;
  s = s->josephus_permutate(3); // <3, 6, 2, 7, 5, 1, 4>
  s = s->josephus_permutate(3); // <2, 1, 6, 4, 5, 3, 7>
  s = s->josephus_permutate(3); // <6, 3, 1, 7, 5, 2, 4>
  s = s->josephus_permutate(3); // <1, 2, 3, 4, 5, 6, 7>
  CPPUNIT_ASSERT_EQUAL(&s1, s);
}
