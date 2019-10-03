#include "kd_tree_t.hpp"
#include <concept_test.hpp>
#include <vector>

// class svgstream;
using namespace std;
using namespace lineda;

CPPUNIT_TEST_SUITE_REGISTRATION(kd_tree_TestCase);

void kd_tree_TestCase::setUp() {
  /**Construct random points */
  vector<point> sinks(200);
  for (auto &sink : sinks) {
    sink = point::random();
  }

  _t1 = new kd_tree;
  _t1->create(sinks);

  _t2 = new kd_tree;
  _t2->create(sinks);

  _t3 = new kd_tree(*_t1);
}

void kd_tree_TestCase::tearDown() {
  delete _t1;
  delete _t2;
  delete _t3;
}

// xxxvoid kd_tree_TestCase::test_copy()
// xxx{
// xxx  CPPUNIT_ASSERT (copy_ok(*_t1));
// xxx  CPPUNIT_ASSERT (copy_ok(*_t2));
// xxx  CPPUNIT_ASSERT (copy_ok(*_t3));
// xxx}
// xxx
// xxx
// xxxvoid kd_tree_TestCase::test_equal_to()
// xxx{
// xxx  CPPUNIT_ASSERT (equal_to_ok(*_t1, *_t2, *_t3));
// xxx}

void kd_tree_TestCase::test_create() {
  /** Test for preset points. */
  point a[] = {point(10, 800), point(20, 600),  point(30, 400), point(40, 200),
               point(50, 100), point(120, 320), point(210, 60), point(560, 30)};
  vector<point> sinks(a, a + 8);
  kd_tree tree;
  tree.create(sinks);
  CPPUNIT_ASSERT(tree._node_count == 15);

  kt_node<point> *top = tree._top;
  tree.report_subtree(top);
  point b[] = {a[2], a[3], a[0], a[1], a[6], a[7], a[4], a[5]};
  vector<point> leaves(b, b + 8); // leaves should be reported.
  CPPUNIT_ASSERT(tree._sinks_selected == leaves);

  point c[] = {point(40, 32767),  point(32767, 400), point(30, 32767),
               point(30, 400),    point(40, 200),    point(10, 32767),
               point(10, 800),    point(20, 600),    point(32767, 60),
               point(210, 32767), point(210, 60),    point(560, 30),
               point(50, 32767),  point(50, 100),    point(120, 320)};
  vector<point> sinks2(c, c + 15);

  vector<point> allnodes; // Check for all nodes in the kd-tree
  for (size_t i = 0; i < 15; ++i) {
    allnodes.push_back(top[i]._data);
  }
  CPPUNIT_ASSERT(allnodes == sinks2);
}

void kd_tree_TestCase::test_search() {
  /** Test for preset points */
  point a[] = {point(10, 800), point(20, 600),  point(30, 400), point(40, 200),
               point(50, 100), point(120, 320), point(210, 60), point(560, 30)};
  vector<point> sinks(a, a + 8);
  kd_tree tree;
  tree.create(sinks);
  for (auto &i : a) {
    tree.select_point(i);
    CPPUNIT_ASSERT(tree._selected_size == 1);
    CPPUNIT_ASSERT(tree._sinks_selected[0] == i);
  }

  //  point b[] = {a[2], a[3], a[0], a[1], a[6], a[7], a[4], a[5]};
  point s1 = point(30, 200);
  point s2 = point(400, 900);
  tree.select_rect(s1, s2);

  vector<point> sinks_select;
  sinks_select.push_back(a[2]);
  sinks_select.push_back(a[3]);
  sinks_select.push_back(a[5]);
  CPPUNIT_ASSERT(tree._sinks_selected == sinks_select);

  /**Test for random points */
  vector<point> rsinks(200);
  for (auto &rsink : rsinks) {
    rsink = point::random();
  }
  kd_tree t;
  t.create(rsinks);
  for (auto &rsink : rsinks) {
    t.select_point(rsink);
    // xxx CPPUNIT_ASSERT (t._selected_size == 1);
    CPPUNIT_ASSERT(t._sinks_selected[0] == rsink);
  }
}
