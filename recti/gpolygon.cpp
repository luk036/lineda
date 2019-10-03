#include "gpolygon.hpp"
#include "rectangle.hpp"
#include "segment.hpp"
#include <algorithm>
#include <functional>
#include <interval.hpp>
#include <svgstream.hpp>
#include <vector>

using numeric::interval;
using dsl::snode;
using std::list;
using std::vector;

namespace recti {
template <>
const char *gpolygon::_svg_style = "fill:green;fill-opacity:0.2;stroke:black";

/** Create randomly a generalized simple polygon with totally n vertices.
    Time complexity O(n^5). */
template <typename _Tp>
GPolygon<_Tp>::GPolygon(size_t n) : _pt_set(n), _tail_list() {
  assert(n > 2);
  typedef snode<Point<_Tp>> pnode;

  // Randomly generate a sequence of points
  pnode *sa = _pt_set.head_ptr();
  pnode *t;
  for (t = sa; t < sa + n; ++t) {
    t->_data = Point<_Tp>::random();
  }

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
        if (rand() & 1) {
          b->_next = a;
          c->reverse();
          c->_next = d;
        } else {
          a->_next = d;
          b->_next = c;
        }

        // Restart
        std::random_shuffle(permute.begin(), permute.end());
        i = 0;
        break;
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

template <> svgstream &operator<<(svgstream &os, const gpolygon &P) {
  typedef dsl::sclist<point> pt_set_type;
  typedef list<snode<point> *> pt_list;
  typedef pt_list::const_iterator It;
  It itt;
  for (itt = P._tail_list.begin(); itt != P._tail_list.end(); ++itt) {
    os << "<polygon"
       << " points=\"";
    const snode<point> *t = *itt;
    do {
      os << " " << t->_data.x() << "," << t->_data.y();
      t = t->_next;
    } while (t != *itt);
    os << " \"";
    os << " style=\"" << gpolygon::_svg_style << "\"";
    os << "/>\n";
  }
  return os;
}

/** Explicit instantiation */
template class GPolygon<int>;
// xxx template class GPolygon<interval<int> >;
}
