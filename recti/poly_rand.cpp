#include "polygon.hpp"
#include "rectangle.hpp"
#include "seg_overlap.hpp"
#include "segment.hpp"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <dsl_algo.hpp>
#include <interval.hpp>
#include <list>
#include <svgstream.hpp>
#include <vector>

using namespace boost::lambda;
using numeric::interval;
using dsl::dnode;
using std::list;
using std::vector;

namespace recti {

/** Create randomly a simple polygon with n vertices.
    Algorithm: generate a random polygon and then apply 2-opt moves
    to pairs of intersecting edges - removing two interesting edges
    and replacing them with two non-intersecting edges so as to keep
    the polygon connected. One can observe that this replacement
    decreases total length and therefore converges to a simple
    polygon. Van Leeuwen and Schoone showed that at most O(n^3) of
    these "untangling 2-opt" moves can be applied, no matter in what
    order they are done. Even though this approach does generate
    each possible simple polygon with some positive probability, it
    does not generate simple polygons *uniformly* at random: some
    polygons have a single permutation that generates them, while
    others have several.

    Reference: C. Zhu et al. Generating Vertices,
    Comput. Geom. Theory Appl., vol. 6, 1996, pp. 277-290.
    Time complexity O(n^5). */
template <> Polygon<int> Polygon<int>::random(size_t n) {
  assert(n > 0);

  // Randomly generate a sequence of points
  pt_set_type pt_set(n);
  dsl::generate_in_cycle(pt_set.begin(), Point<int>::random);
  // xxx pnode *sa = pt_set.head_ptr();

  bool intersect;
  Polygon<int> res(pt_set);
  seg_overlap_sweeper sweeper(res);

  do {
    intersect = sweeper.scan();
  } while (intersect);

  // Reconnect the doubly linked list
  res._pt_set.connect();
  assert(res._pt_set.is_valid());
  // xxx Polygon<int> res(pt_set);
  res._pt_set.set_tail(res._pt_set.head_ptr());
  res.normalize();
  return res;
}

// xxx template <>
// xxx Polygon<int> Polygon<int>::random(size_t n)
// xxx {
// xxx   assert(n > 0);
// xxx
// xxx   // Randomly generate a sequence of points
// xxx   pt_set_type pt_set(n);
// xxx   dsl::generate_in_cycle(pt_set.begin(), Point<int>::random);
// xxx   pnode *sa = pt_set.head_ptr();
// xxx
// xxx   bool intersect;
// xxx
// xxx   do {
// xxx     size_t i, j;
// xxx     intersest = false;
// xxx     //xxx std::random_shuffle(permute.begin(), permute.end());
// xxx
// xxx     // Untangle the edge intersecting segments
// xxx     for (i=0; i<n-1; ++i) {
// xxx 	//xxx pnode *a = permute[i];
// xxx 	pnode *a = sa + i;
// xxx 	pnode *c = a->_next;
// xxx 	const Segment<double> ac(a->_data, c->_data);
// xxx
// xxx 	for (j=i+1; j<n; ++j) {
// xxx 	  //xxx pnode *b = permute[j];
// xxx 	  pnode *b = sa + j;
// xxx 	  pnode *d = b->_next;
// xxx 	  if (a->_data == b->_data || a->_data == d->_data
// xxx 	      || c->_data == b->_data || c->_data == d->_data) continue;
// xxx
// xxx 	  const Segment<double> bd(b->_data, d->_data);
// xxx 	  if (ac.overlap(bd)) {
// xxx 	    // 2-opt
// xxx 	    b->_next = a;
// xxx 	    c->reverse();
// xxx 	    c->_next = d;
// xxx           intersect = true;
// xxx           break;
// xxx 	  }
// xxx 	}
// xxx     }
// xxx   } while (intersect);
// xxx
// xxx   // Reconnect the doubly linked list
// xxx   pt_set.connect();
// xxx
// xxx   assert(pt_set.is_valid());
// xxx   Polygon<int> res(pt_set);
// xxx   res._pt_set.set_tail(res._pt_set.head_ptr());
// xxx   res.normalize();
// xxx   return res;
// xxx }

/** Explicit instantiation */
template Polygon<int> Polygon<int>::random(size_t n);
}
