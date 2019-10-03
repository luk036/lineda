/* Generalized Rectilinear Polygon */
#include "grpolygon.hpp"
#include "rectangle.hpp"
#include "rpolygon.hpp"
#include "segment.hpp"
#include <interval.hpp>
#include <list>
#include <svgstream.hpp>
#include <vector>

using numeric::interval;
using numeric::make_interval;
using dsl::snode;
using std::list;
using std::vector;

namespace recti {
template <>
const char *grpolygon::_svg_style = "fill:green;fill-opacity:0.2;stroke:black";

/** Create randomly a set of simple rectilinear polygons with
    totally n vertices randomly.
    Reference: ???
    Time complexity O(n^5). */
template <typename _Tp>
GRPolygon<_Tp>::GRPolygon(size_t n)
    : _pt_set(RPolygon<_Tp>::random1(n)), _tail_list() {
  assert(n > 2);
  assert(n % 2 == 0);

  // xxx typedef snode<Point<_Tp> > pnode;
  pnode *sa = _pt_set.head_ptr();

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
    const Point<_Tp> &pa = a->_data;
    Point<_Tp> &pc = c->_data;

    size_t j = i;
    while (j < n) {
      pnode *b = permute[j];
      pnode *d = b->_next;
      const Point<_Tp> &pb = b->_data;
      Point<_Tp> &pd = d->_data;

      if (pa == pb || pa == pd || pc == pb || pc == pd) {
        ++j;
        continue;
      }

      if (pa.is_horizontal(pc)) {
        if (pb.is_vertical(pd)) {
          if (make_interval(pa.x(), pc.x()).contain(pb.x()) &&
              make_interval(pb.y(), pd.y()).contain(pa.y())) { // overlap
            pnode *e = c->_next;
            pnode *f = d->_next;
            if (rand() & 7) {
              b->_next = c;
              c->reverse();
              e->_next = d;
              pc = Point<_Tp>(pb.x(), pa.y());
              pd = Point<_Tp>(e->_data.x(), f->_data.y());
            } else {
              b->_next = c;
              a->_next = d;
              pc = Point<_Tp>(e->_data.x(), pb.y());
              pd = Point<_Tp>(pa.x(), f->_data.y());
            }

            // Restart
            std::random_shuffle(permute.begin(), permute.end());
            i = 0;
            break;
          }
        } else { // both horizontal
          if (pa.y() == pb.y() &&
              make_interval(pa.x(), pc.x())
                  .overlap(make_interval(pb.x(), pd.x()))) { // overlap
            const _Tp d_ac = pa.x() - pc.x();
            const _Tp d_bd = pb.x() - pd.x();
            if ((d_ac > 0 && d_bd > 0) || (d_ac < 0 && d_bd < 0)) { // same dir
              b->_next = a;
              c->reverse();
              c->_next = d;
            } else {
              b->_next = c;
              a->_next = d;
            }

            // Restart
            std::random_shuffle(permute.begin(), permute.end());
            i = 0;
            break;
          }
        }
      } else {
        if (pb.is_horizontal(pd)) {
          if (make_interval(pa.y(), pc.y()).contain(pb.y()) &&
              make_interval(pb.x(), pd.x()).contain(pa.x())) { // overlap
            pnode *e = c->_next;
            pnode *f = d->_next;
            if (rand() & 7) {
              b->_next = c;
              c->reverse();
              e->_next = d;
              pc = Point<_Tp>(pa.x(), pb.y());
              pd = Point<_Tp>(f->_data.x(), e->_data.y());
            } else {
              b->_next = c;
              a->_next = d;
              pc = Point<_Tp>(pb.x(), e->_data.y());
              pd = Point<_Tp>(f->_data.x(), pa.y());
            }

            // Restart
            std::random_shuffle(permute.begin(), permute.end());
            i = 0;
            break;
          }
        } else { // both vertical
          if (pa.x() == pb.x() &&
              make_interval(pa.y(), pc.y())
                  .overlap(make_interval(pb.y(), pd.y()))) { // overlap
            const _Tp d_ac = pa.y() - pc.y();
            const _Tp d_bd = pb.y() - pd.y();
            if ((d_ac > 0 && d_bd > 0) || (d_ac < 0 && d_bd < 0)) { // same dir
              b->_next = a;
              c->reverse();
              c->_next = d;
            } else {
              b->_next = c;
              a->_next = d;
            }

            // Restart
            std::random_shuffle(permute.begin(), permute.end());
            i = 0;
            break;
          }
        }
      }
      ++j;
    }
    ++i;
  }

  // Mark the polygons
  std::vector<bool> visited(n, false);
  for (size_t i = 0; i < n; ++i) {
    if (visited[i])
      continue;
    pnode *tail = sa + i;
    _tail_list.push_back(tail);
    const pnode *t = tail;
    do {
      visited[t - sa] = true;
      t = t->_next;
    } while (t != tail);
  }
}

template <> svgstream &operator<<(svgstream &os, const grpolygon &P) {
  typedef dsl::dclist<point> pt_set_type;
  typedef pt_set_type::node pnode;
  typedef list<pnode *> pt_list;
  typedef pt_list::const_iterator It;

  It itt;
  for (itt = P._tail_list.begin(); itt != P._tail_list.end(); ++itt) {
    os << "<polygon"
       << " points=\"";
    const pnode *t = *itt;
    do {
      os << " " << t->_data.x() << "," << t->_data.y();
      t = t->_next;
    } while (t != *itt);
    os << " \"";
    os << " style=\"" << grpolygon::_svg_style << "\"";
    os << "/>\n";
  }
  return os;
}

/** Explicit instantiation */
template class GRPolygon<int>;
}
