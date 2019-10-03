#include "polygon.hpp"
#include "geo_iter.hpp"
#include "rectangle.hpp"
#include "segment.hpp"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <dsl_algo.hpp>
#include <interval.hpp>
#include <iostream>
#include <list>
#include <svgstream.hpp>
#include <vector>

using namespace boost::lambda;
using namespace numeric;
using numeric::interval;
using dsl::dnode;
using std::list;
using std::vector;

namespace recti {
template <>
const char *polygon::_svg_style = "fill:green;fill-opacity:0.2;stroke:black";

/** Minkowski sum with a point q and assign */
template <typename _Tp>
template <typename _Up>
Polygon<_Tp> &Polygon<_Tp>::operator+=(const Point<_Up> &q) {
  _pt_set.apply(_1 += var(q));
  return *this;
}

/** @return whether this polygon is clockwise. Basic idea: first
    find the left-bottom-most corner and determine if it is turning
    left. Time complexit O(N). Precondition: no duplicate points */
template <typename _Tp> bool Polygon<_Tp>::is_clockwise() const {
  if (_pt_set.size() < 3)
    return true;

  // Find the left-bottom-most point
  const const_iterator lbmpt = min_element_in_cycle(_pt_set.begin());

  // Left turn or right turn
  return area2(*lbmpt.prev(), *lbmpt, *lbmpt.next()) < 0;
}

/** Remove duplicates of consecutive horizontal/vertical segments */
template <typename _Tp> void Polygon<_Tp>::unique_segment() {
  if (_pt_set.tail() == _pt_set.tail()->_next)
    return; // only one node
  pnode *s = _pt_set.tail();
  do {
    pnode *t = s->_next;
    pnode *r = s->_prev;
    while (s->_data.is_vertical(t->_data) && s->_data.is_vertical(r->_data) ||
           s->_data.is_horizontal(t->_data) &&
               s->_data.is_horizontal(r->_data)) {
      _pt_set.detach(s);
      if (s == _pt_set.tail()) {
        _pt_set.set_tail(r);
      }
      s = t;
      t = s->_next;
    }
    s = t;
  } while (s != _pt_set.tail());
}

/** Normalize polygon in canonical form */
template <typename _Tp> void Polygon<_Tp>::normalize() {
  _pt_set.unique(); // remove duplicates of consecutive elements
  unique_segment(); // remove duplicates of consecutive V/H segments

  // Find the left-bottom-most point
  const iterator lbmpt = min_element_in_cycle(_pt_set.begin());

  // Left turn or right turn
  if (area2(*lbmpt.prev(), *lbmpt, *lbmpt.next()) < 0)
    _pt_set.reverse();

  // Make left-bottom-most point as the first point
  _pt_set.set_tail(lbmpt.prev().value());
  assert(_pt_set.is_valid());
}

/** @return direction of two consecutive segments */
template <typename _Iterator>
inline typename std::iterator_traits<_Iterator>::value_type::value_type
get_direction(const _Iterator &it) {
  return det(*it.next() - *it, *it.next().next() - *it);
}

/** @return whether this polygon is convex. (not tested yet) */
template <typename _Tp> bool Polygon<_Tp>::is_convex() const {
  if (_pt_set.size() <= 3)
    return true;

  const_iterator it = _pt_set.begin();
  const_iterator it_start = it;

  while (get_direction(it) == 0)
    ++it;
  if (get_direction(it) > 0) { // turn left
    for (++it; it != it_start; ++it) {
      if (get_direction(it) < 0)
        return false;
    }
  } else { // turn right
    for (++it; it != it_start; ++it) {
      if (get_direction(it) > 0)
        return false;
    }
  }
  return true;
}

/** @return whether this polygon is x-monotone (not tested yet) */
template <typename _Tp>
template <class GEOIterator>
bool Polygon<_Tp>::is_x_monotone_internal(GEOIterator it) const {
  if (_pt_set.size() <= 3)
    return true;
  GEOIterator it_start = it;

  while (it.x() == it.next().x())
    ++it;
  if (it.x() > it.next().x()) {
    for (++it; it.x() >= it.next().x(); ++it)
      ; // find leftmost node
    for (++it; it.x() <= it.next().x(); ++it)
      ; // find rightmost node
    if (it == it_start || it.next() == it_start)
      return true;
    for (++it; it != it_start; ++it) {
      if (it.x() < it.next().x())
        return false;
    }
  } else {
    for (++it; it.x() <= it.next().x(); ++it)
      ; // find rightmost node
    for (++it; it.x() >= it.next().x(); ++it)
      ; // find leftmost node
    if (it == it_start || it.next() == it_start)
      return true;
    for (++it; it != it_start; ++it) {
      if (it.x() > it.next().x())
        return false;
    }
  }
  return true;
}

/** @return whether this polygon is x-monotone (not tested yet) */
template <typename _Tp> bool Polygon<_Tp>::is_x_monotone() const {
  const_iterator ii = _pt_set.begin();
  geo_iterator<const_iterator> it(ii);
  return is_x_monotone_internal(it);
}

/** @return whether this polygon is x-monotone (not tested yet) */
template <typename _Tp> bool Polygon<_Tp>::is_y_monotone() const {
  const_iterator ii = _pt_set.begin();
  geo_dual_iterator<const_iterator> it(ii);
  return is_x_monotone_internal(it);
}

/**
 * Create randomly a simple polygon with n vertices.
 * Algorithm: generate a random polygon and then apply 2-opt moves
 * to pairs of intersecting edges - removing two interesting edges
 * and replacing them with two non-intersecting edges so as to keep
 * the polygon connected. One can observe that this replacement
 * decreases total length and therefore converges to a simple
 * polygon. Van Leeuwen and Schoone showed that at most O(n^3) of
 * these "untangling 2-opt" moves can be applied, no matter in what
 * order they are done. Even though this approach does generate
 * each possible simple polygon with some positive probability, it
 * does not generate simple polygons *uniformly* at random: some
 * polygons have a single permutation that generates them, while
 * others have several.
 *
 * Reference: C. Zhu et al. Generating Vertices,
 * Comput. Geom. Theory Appl., vol. 6, 1996, pp. 277-290.
 * Time complexity O(n^5).
 */
template <typename _Tp> Polygon<_Tp> Polygon<_Tp>::random2(size_t n) {
  assert(n > 0);

  // Randomly generate a sequence of points
  pt_set_type pt_set(n);
  dsl::generate_in_cycle(pt_set.begin(), Point<_Tp>::random);
  pnode *sa = pt_set.head_ptr();

  vector<pnode *> permute(n);
  for (size_t i = 0; i < n; ++i) {
    permute[i] = sa + i; // for randomizing the order later
  }
  std::random_shuffle(permute.begin(), permute.end());

  // Untangle the edge intersecting segments
  size_t i = 1;
  while (i < n) {
    pnode *a = permute[i - 1];
    pnode *c = a->_next;
    const Segment<double> ac(a->_data, c->_data);

    size_t j = i;
    while (j < n) {
      pnode *b = permute[j];
      pnode *d = b->_next;
      if (a->_data == b->_data || a->_data == d->_data ||
          c->_data == b->_data || c->_data == d->_data) {
        ++j;
        continue;
      }
      const Segment<double> bd(b->_data, d->_data);
      if (ac.overlap(bd)) {
        // 2-opt
        b->_next = a;
        c->reverse();
        c->_next = d;

        // Restart
        std::random_shuffle(permute.begin(), permute.end());
        i = 0;
        break;
      }
      ++j;
    }
    ++i;
  }

  // Reconnect the doubly linked list
  pt_set.connect();

  assert(pt_set.is_valid());
  Polygon<_Tp> res(pt_set);
  res.normalize();
  return res;
}

/* Create a polygon by an array of points. */
template <> Polygon<int>::Polygon(const point *a, size_t s) : _pt_set(s) {
  pt_set_type::iterator it = _pt_set.begin();
  do {
    *it++ = *a++;
  } while (it != _pt_set.begin());
}

/** Create a polygon by converting a rectangle r. */
template <> Polygon<int>::Polygon(const Rectangle<int> &r) : _pt_set(4) {
  const point ll = r.ll();
  const point ur = r.ur();

  pt_set_type::iterator it = _pt_set.begin();
  *it++ = ll;
  *it++ = point(ur.x(), ll.y());
  *it++ = ur;
  *it = point(ll.x(), ur.y());
}

template <>
template <typename _Up>
Polygon<int> &Polygon<int>::operator+=(const Rectangle<_Up> &r) {
  if (num_vertices() == 0)
    return *this; // empty polygon

  const polygon pr(r);
  pt_set_type::const_iterator it = pr._pt_set.begin();
  pt_set_type::iterator it1 = _pt_set.begin();
  pt_set_type::iterator it2 = it1;
  ++it1;

  for (; !it2->below(*it1); ++it1, ++it2)
    *it2 += *it;
  ++it;
  for (; !it2->left_of(*it1); ++it1, ++it2)
    *it2 += *it;
  ++it;
  for (; !it1->below(*it2); ++it1, ++it2)
    *it2 += *it;
  ++it;
  do {
    *it2++ += *it++;
  } while (it2 != _pt_set.begin());
  return *this;
}

template <> svgstream &operator<<(svgstream &os, const polygon &P) {
  typedef polygon::pt_set_type pt_set_type;
  os << "<polygon"
     << " points=\"";
  pt_set_type::const_iterator it = P._pt_set.begin();
  do
    os << " " << it->x() << "," << it->y();
  while (++it != P._pt_set.begin());
  os << " \"";
  os << " style=\"" << polygon::_svg_style << "\"";
  os << "/>";
  return os;
}

template <> std::ostream &operator<<(std::ostream &os, const Polygon<int> &P) {
  os << "(";
  // Output each vertex's coordinate
  dsl::for_each_in_cycle(P._pt_set.begin(), os << constant(" ") << _1);
  os << ")";
  return os;
}

/** Explicit instantiation */
template class Polygon<int>;
}

void dummy_svgstream() {
  svgstream t("test.svg");
  recti::Polygon<int> P = recti::Polygon<int>::random(6);
  t << P;
  t << '\n';
}
