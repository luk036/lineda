#include "rpolygon.hpp"
#include "rectangle.hpp"
#include "segment.hpp"
#include <interval.hpp>
#include <svgstream.hpp>
#include <vector>

using numeric::interval;
using numeric::make_interval;
using dsl::snode;
using std::vector;

namespace recti {
template <>
const char *rpolygon::_svg_style = "fill:green;fill-opacity:0.2;stroke:black";

template <typename _Tp> inline Point<int> sign(const numeric::vector2<_Tp> &p) {
  int x = 0, y = 0;
  if (p._a1 > 0)
    x = 1;
  else if (p._a1 < 0)
    x = -1;
  if (p._a2 > 0)
    y = 1;
  else if (p._a2 < 0)
    y = -1;
  return Point<int>(x, y);
}

/** Create a sequence of points with n vertices randomly.
    Time complexity O(n). */
template <typename _Tp>
typename RPolygon<_Tp>::pt_set_type RPolygon<_Tp>::random1(size_t n) {
  assert(n > 3);
  assert(n % 2 == 0);

  typedef typename RPolygon<_Tp>::pnode pnode;

  // Randomly generate a sequence of points
  pt_set_type pt_set(n);
  pnode *tail = pt_set.tail();
  pnode *a = tail;

  do {
    a->_data = Point<_Tp>::random();
    a = a->_next->_next;
  } while (a != tail);

  do {
    pnode *t = a->_next;
    t->_data = Point<_Tp>(t->_next->_data.x(), a->_data.y());
    a = t->_next;
  } while (a != tail);

  return pt_set;
}

/** Create a simple rectilinear polygon with n vertices randomly.
    Time complexity O(n^5). */
template <typename _Tp> RPolygon<_Tp> RPolygon<_Tp>::random(size_t n) {
  assert(n > 3);
  assert(n % 2 == 0);

  // Randomly generate a sequence of points
  pt_set_type pt_set = RPolygon<_Tp>::random1(n);

  vector<pnode *> permute(n);
  pnode *sa = pt_set.head_ptr();
  for (size_t i = 0; i < n; ++i) {
    permute[i] = sa + i; // for randomizing the order later
  }

  size_t count = 0; // count how many 2-opts
  // xxx size_t max_iter = n*n*n*n*n;
  bool interest;
  // size_t count = 0;

  do {
    size_t i, j;
    interest = false;
    std::random_shuffle(permute.begin(), permute.end());

    // Untangle the edge intersecting segments
    for (i = 0; i < n - 1; ++i) {
      pnode *a = permute[i];
      pnode *c = a->_next;
      const Point<_Tp> &pa = a->_data;
      Point<_Tp> &pc = c->_data;

      for (j = i + 1; j < n; ++j) {
        pnode *b = permute[j];
        pnode *d = b->_next;
        if (a->_data == b->_data || a->_data == d->_data ||
            c->_data == b->_data || c->_data == d->_data)
          continue;

        const Point<_Tp> &pb = b->_data;
        Point<_Tp> &pd = d->_data;

        if (pa.is_horizontal(pc)) {
          if (pb.is_vertical(pd)) {
            if (make_interval(pa.x(), pc.x()).contain(pb.x()) &&
                make_interval(pb.y(), pd.y()).contain(pa.y())) { // overlap
              pnode *e = c->_next;
              pnode *f = d->_next;
              b->_next = c;
              c->reverse();
              e->_next = d;
              pc = Point<_Tp>(pb.x(), pa.y());
              pd = Point<_Tp>(e->_data.x(), f->_data.y());
              ++count;
              interest = true;
              break;
            }
          } else { // both horizontal
            if (pa.y() == pb.y() &&
                make_interval(pa.x(), pc.x())
                    .overlap(make_interval(pb.x(), pd.x()))) { // overlap
              const _Tp d_ac = pa.x() - pc.x();
              const _Tp d_bd = pb.x() - pd.x();
              if ((d_ac > 0 && d_bd > 0) ||
                  (d_ac < 0 && d_bd < 0)) { // same dir
                b->_next = a;
                c->reverse();
                c->_next = d;
                ++count;
                interest = true;
                break;
              }
            }
          }
        } else { // same as if clause with x <=> y
          if (pb.is_horizontal(pd)) {
            if (make_interval(pa.y(), pc.y()).contain(pb.y()) &&
                make_interval(pb.x(), pd.x()).contain(pa.x())) { // overlap
              pnode *e = c->_next;
              pnode *f = d->_next;
              b->_next = c;
              c->reverse();
              e->_next = d;
              pc = Point<_Tp>(pa.x(), pb.y());
              pd = Point<_Tp>(f->_data.x(), e->_data.y());
              ++count;
              interest = true;
              break;
            }
          } else { // both vertical
            if (pa.x() == pb.x() &&
                make_interval(pa.y(), pc.y())
                    .overlap(make_interval(pb.y(), pd.y()))) { // overlap
              const _Tp d_ac = pa.y() - pc.y();
              const _Tp d_bd = pb.y() - pd.y();
              if ((d_ac > 0 && d_bd > 0) ||
                  (d_ac < 0 && d_bd < 0)) { // same dir
                b->_next = a;
                c->reverse();
                c->_next = d;
                ++count;
                interest = true;
                break;
              }
            }
          }
        }
      }
    }
  } while (interest);

  // Reconnect the doubly linked list
  pt_set.connect();

  assert(pt_set.is_valid());
  RPolygon<_Tp> res(pt_set);
  res._pt_set.set_tail(res._pt_set.head_ptr());
  // res.normalize();
  return res;
}

template <typename _Tp> void RPolygon<_Tp>::shrink(const _Tp &delta) {
  _Base::normalize();
  iterator it = _Base::_pt_set.begin();
  ++it;                    // skip the first vertex
  const iterator it2 = it; // it2 points to the leftmost bottom vertex
  Point<int> p(+1, +1);
  // xxx Point<_Tp> check = *it + p*delta;
  Point<_Tp> p0 = *it++;
  Point<_Tp> p1 = *it;
  do {
    const Point<_Tp> p2 = *it.next();
    Point<int> d = sign(p2 - p0);
    d.rotate090();
    if (p0.is_horizontal(p1)) {
      if (p.y() > 0 != d.y() > 0)
        d.rotate180();
    } else { // vertical
      if (p.x() > 0 != d.x() > 0)
        d.rotate180();
    }
    *it += d * delta;

    // xxx if (recti::area2(p0,p1,p2) > 0) {
    // xxx   if (d.x() != p.x()) {
    // xxx     Point<_Tp> pa = *it.prev().prev();
    // xxx     //xxx _Tp temp = std::min(distance(p1,p2), distance(p0,pa));
    // xxx     //xxx if (recti::distance(p0,p1) < temp) {
    // xxx     it->translate(0, -d.y()*delta);
    // xxx     it.prev()->translate(0, -d.y()*delta);
    // xxx     check.translate(0, -d.y()*delta);
    // xxx     //xxx }
    // xxx   }
    // xxx   else if (d.y() != p.y()) {
    // xxx     Point<_Tp> pa = *it.prev().prev();
    // xxx     //xxx _Tp temp = std::min(distance(p1,p2), distance(p0,pa));
    // xxx     //xxx if (recti::distance(p0,p1) < temp) {
    // xxx     it->translate( -d.x()*delta, 0);
    // xxx     it.prev()->translate(-d.x()*delta, 0);
    // xxx     check.translate(-d.x()*delta, 0);
    // xxx     // xxx}
    // xxx   }
    // xxx }
    // xxx assert(check.x() == it->x() || check.y() == it->y());
    // xxx check = *it;
    p0 = p1;
    p1 = p2;
    p = d;
  } while (++it != it2);

  *it += Point<int>(+1, +1) * delta;
}

/** Explicit instantiation */
template class RPolygon<int>;
}
