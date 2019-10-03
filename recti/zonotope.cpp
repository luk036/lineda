#include "zonotope.hpp"
#include <aa.h>
#include <algorithm>                  // for std::set_union()
#include <boost/lambda/bind.hpp>      // for bind()
#include <boost/lambda/construct.hpp> // for constructor()
#include <boost/lambda/lambda.hpp>    // for _1, _2 ...
#include <matrix2.hpp>
#include <numeric> // for std::accumulate()
#include <svgstream.hpp>
#include <vector2.hpp>

using namespace boost::lambda;
using numeric::vector2;
using numeric::matrix2;

namespace recti {
template <typename _Tp>
const char *Zonotope<_Tp>::_svg_style =
    "fill:green;fill-opacity:0.5;stroke:black";

/** Create a zonotope by AAF */
template <>
template <>
Zonotope<double>::Zonotope(const Point<AAF> &p)
    : _center(p.x().get_center(), p.y().get_center()) {
  const size_t lx = p.x().get_length();
  const size_t ly = p.y().get_length();
  const unsigned int *idx_begin = p.x().indexes;
  const unsigned int *idy_begin = p.y().indexes;
  const unsigned int *idx_end = idx_begin + lx;
  const unsigned int *idy_end = idy_begin + ly;

  // Fill the resulting indexes array
  // by merging the 2 input indexes array

  std::vector<size_t> idf(lx + ly);
  auto idf_end =
      std::set_union(idx_begin, idx_end, idy_begin, idy_end, idf.begin());
  size_t lf = std::distance(idf.begin(), idf_end);

  _pt_set.resize(lf);
  const unsigned int *pux = idx_begin;
  const unsigned int *puy = idy_begin;
  const double *vax = p.x().coefficients;
  const double *vay = p.y().coefficients;

  for (size_t i = 0; i < lf; ++i) {
    if (pux == idx_end || *pux != idf[i]) {
      _pt_set[i] = numeric::vector2<double>(0, *vay++);
      ++puy;
    } else if (puy == idy_end || *puy != idf[i]) {
      _pt_set[i] = numeric::vector2<double>(*vax++, 0);
      ++pux;
    } else {
      _pt_set[i] = numeric::vector2<double>(*vax++, *vay++);
      ++pux;
      ++puy;
    }
  }

  normalize();
}

/** Create a zontope with random size at random location. */
template <> Zonotope<int> Zonotope<int>::random() {
  pt_set_type v((rand() & 7) + 2);
  std::generate(v.begin(), v.end(), numeric::vector2<int>::random);
  std::for_each(v.begin(), v.end(), _1 /= 10);
  return _Self(Point<int>::random() / 2, v);
}

/**
 * Approximate a given zonotope w by means of zonotopes from the
 * class of this zonotope (z), so that w is an outer
 * approximation of z, i.e. w \subset z
 *
 * Algorithm. Given are unit vector p(i)=(cos(chi(i)),sin(chi(i))),
 * i = 1,...,m, such that 0 <= chi(i) < chi(i+1) < pi, and
 * nonegative numbers rho(i) >= 0, i=1,..,m, defining a zonotope w =
 * \sum_{i=1}^m rho(i)*p~(i). We want to find suitable values
 * {eps(i)}_{i=1}^k such that the zonotope z = \sum_{i=1}^k
 * eps(i)*e~(i) is an outer approximation of w, that is w \subset z.
 * We present the vector p(i) = (cos(chi(i)),sin(chi(i))) as
 *     p(i) = eps(i1)e(j) + eps(i2)e(j+1), j=j(i), i=1,...,m   (9)
 * where e(j), e(j+1) are the nearest basic unit vectors enclosing
 * p(i) with phi(j) <= chi(i) <= phi(j+1) and eps(i1), eps(i2) are
 * some nonnegative coeficients. Clearly, relations (9) define the
 * coefficients eps(i1), eps(i2) in a unique way. We note that if
 * some of the equalities phi(j) = chi(i), chi(i) = phi(j+1) takes
 * place, then one of the coefficients eps(i1), eps(i2) will be
 * equal to zero.
 * Relations (9) imply the inclusions p~(i) \subset eps(i1)*e~(j) +
 * eps(i2)*e~(j+1), i=1,...,m. Subsequently we obtain
 *   w = \sum_{i=1}^m rho(i)*p~(i)
 *     \subset \sum_{i=1}^m rho(i) * (eps(i1)*e~(j) + eps(i2)*e~(j+1))
 *     \subset \sum_{i=1}^k eps(i)*e~(i)
 *     = z
 * with some eps(i) >= 0 that can be effectively computed.
 * It can be shown that the above algorithm produces an optimal
 * approximation as regard the Hausdorff/integral metric. Roughly
 * speaking this follows from the fact that every single segment
 * p~(i) has been optimally approximated.
 *
 * Preconditions:
 *  Both z and w are in canonical form.
 *  z at least two basis segments (i.e. at least 4 vertices)
 *
 * Reference: S. Markov and D. Claudio. On the Approximation of
 * Centered Zonotope in the Plane, I. Lirkov et al. (Eds.) LSSC 2003,
 * Springer-Verlag, LNCS 2907, pp. 246-253, 2004.
 */
template <typename _Tp>
Zonotope<_Tp> Zonotope<_Tp>::approximate(const Zonotope<_Tp> &w) const {
  assert(is_normal() && w.is_normal());
  assert(_pt_set.size() >= 2);

  const size_t m = w._pt_set.size();
  const size_t k = _pt_set.size();

  Compare_By_Angle<double> compare;
  std::vector<double> eps_set(k, 0.0);
  size_t j = 0;
  for (size_t i = 0; i < m; ++i) {
    const numeric::vector2<double> p = w._pt_set[i];

    // Find the nearest pair
    while (j < k && compare(_pt_set[j], p))
      ++j;
    const size_t j1 = (j == 0) ? k - 1 : j - 1;
    const size_t j2 = (j == k) ? 0 : j;
    const numeric::vector2<double> e1 = _pt_set[j1]; // e(j)
    const numeric::vector2<double> e2 = _pt_set[j2]; // e(j+1)

    if (!compare(p, e2) && !compare(e2, p)) { // i.e. p is equivalent to v
      const double eps = (e2._a2 != 0) ? p._a2 / e2._a2 : p._a1 / e2._a1;
      eps_set[j2] += fabs(eps);
    } else {
      // const vector2<double> pv(p.x(), p.y());
      const matrix2<double> A(e1._a1, e2._a1, e1._a2, e2._a2);
      const vector2<double> eps = p / A;
      eps_set[j1] += fabs(eps._a1);
      eps_set[j2] += fabs(eps._a2);
    }
  }

  Zonotope<_Tp> res(*this);
  res._center = w._center;
  for (size_t i = 0; i < k; ++i) {
    res._pt_set[i] *= eps_set[i];
  }
  return res;
}

/**
 * Draw the zonotope in svg format.
 * For a_i >= 0 the vectors s^(i) = a_i e^(i) = (a_i
 * cos phi_i, sin phi_i) induce the centered segments. The positive
 * combination of the segments is a centered zonotope with 2k
 * vertices: t1, t2, ..., t(k), -t1, -t2, ..., -t(k), where
 *
 *   t1 =  a1 e1 + a2 e2 +      ...     + a(k-1)e(k-1) + a(k)e(k),
 *   t2 = -a1 e1 + a2 e2 +      ...     + a(k-1)e(k-1) + a(k)e(k),
 *   . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 *   ti = -a1 e1 - ... - a(i-1)e(i-1) + a(i)e(i) + ... + a(k)e(k),
 *   . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
 *   tk = -a1 e1 + a2 e2 -      ...     - a(k-1)e(k-1) + a(k)e(k).
 *
 * The vertices t1, t2, ..., t(k) are lying in cyclic anticlockwise
 * order in a half-plane between the vectors t(1) = -t(1) + 2*a(k)*e(k).
 *
 * Reference: S. Markov and D. Claudio. On the Approximation of
 * Centered Zonotope in the Plane, I. Lirkov et al. (Eds.) LSSC 2003,
 * Springer-Verlag, LNCS 2907, pp. 246-253, 2004.
 */
template <typename _Tp> svgstream &operator<<(svgstream &os, Zonotope<_Tp> z) {
  typename Zonotope<_Tp>::pt_set_type &V = z._pt_set;
  Point<_Tp> t = std::accumulate(V.begin(), V.end(), z.center());
  std::for_each(V.begin(), V.end(), _1 *= 2);

  // Draw the polygon
  os << "<polygon"
     << " points=\"";
  std::for_each(
      V.begin(), V.end(),
      (t -= _1, os << constant(" ") << var(t.x()) << "," << var(t.y())));
  std::for_each(
      V.begin(), V.end(),
      (t += _1, os << constant(" ") << var(t.x()) << "," << var(t.y())));
  os << " \"";
  os << " style=\"" << Zonotope<_Tp>::_svg_style << "\"";
  os << "/>\n";

  // Draw the vertices (by a circle)
  std::for_each(V.begin(), V.end(), os << (t -= _1) << '\n');
  std::for_each(V.begin(), V.end(), os << (t += _1) << '\n');

  return os;
}

/** Explicit instantiation */
template Zonotope<double>
Zonotope<double>::approximate(const Zonotope<double> &w) const;

template Zonotope<double>::Zonotope(const Point<AAF> &p);

template const char *Zonotope<int>::_svg_style;
template const char *Zonotope<double>::_svg_style;

template svgstream &operator<<(svgstream &os, Zonotope<int> z);
template svgstream &operator<<(svgstream &os, Zonotope<double> z);
}
