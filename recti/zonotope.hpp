#ifndef RECTI_ZONOTOPE_HPP
#define RECTI_ZONOTOPE_HPP

#include "point.hpp"
#include "polygon.hpp"
#include "rectangle.hpp"

#include <algorithm>
#include <cassert>
#include <cmath> // for fabs()
#include <functional>
#include <iosfwd>
#include <utility>
#include <vector>

class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/** Forward declarations */
template <typename _Tp> class Zonotope;
template <typename _Tp> class Rectangle;
template <typename _Tp>
std::ostream &operator<<(std::ostream &os, Zonotope<_Tp> z);
template <typename _Tp> svgstream &operator<<(svgstream &os, Zonotope<_Tp> z);

/** Define integer coordinate type zonotope */
typedef Zonotope<int> zonotope;

/** Compare by angle */
template <typename _Tp> class Compare_By_Angle {
public:
  /** @return true if angle(p) < angle(q) */
  bool operator()(const numeric::vector2<_Tp> &v,
                  const numeric::vector2<_Tp> &w) const {
    return det(v, w) < 0;
  }
};

/**
 * Zonotope Class. Zonotopes are convex bodies with simple algebraic
 * presentation: they are Minkowski sums of segments. By means of a
 * translation (addition of a vector) any zonotope can be centered at
 * the origin, therefore without loss of generality we can restrict
 * our considerations to centered (origin symmetric) zonotopes. The
 * latter are positive combinations of centered unit segments. A fixed
 * system of centered unit segments generates a class of zonotopes
 * consisting of all positive combinations of the unit segments. This
 * class is closed under addition and multiplication by scalar and
 * forms a quasilinear space. A quasilinear space over the field of
 * reals is an additve abelian monoid with cancellation law endowed
 * with multiplication by scalars. Any quasilinear space can be
 * embedded in a group; in addition a natural isomorphic extension of
 * the multiplication by scalars leads to quasilinear spaces with
 * group structure called quasivector spaces. Quasivector spaces obey
 * all axioms of vector spaces, but in the place of the second
 * distributive law we have: (alpha + beta)*c == alpha*c + beta*c, if
 * alpha*beta >= 0. If, in addition, the elements satisfy the
 * relation: (-1)*c == c, then the space is called symmetric
 * quasilinear space.
 * Every quasilinear space is a direct sum of a vector space and a
 * symmetric quasivector space. On the othe side, the algebraic
 * operations in vector and symmetric quasivector spaces are mutually
 * representable. This enables us to transfer basic vector space
 * concepts (such as linear combination, basis, dimension, etc.) to
 * symmetric quasivector spaces. Let us also mention that symmetric
 * quasivector spaces with finite basis are isomorphic to a canonic
 * space similar to R^n. These results can be used for computations
 * with zonotopes as then we practically work in a vector
 * space. Computing with centered zonotopes is especially simple and
 * instructive.
 *
 * Reference:
 * [1] Svetoslav Markov and Dalcidio Claudio, On the Approximation of
 * Centered Zonotopes in the Plane, I. Lirkov e al. (Eds.): LSSC 2003,
 * Springer-Verlag, LNCS 2907, pp. 246-253, 2004.
 */
template <typename _Tp> class Zonotope {
  typedef Zonotope<_Tp> _Self;
  friend std::ostream &operator<<<>(std::ostream &, _Self);
  friend svgstream &operator<<<>(svgstream &, _Self);

public:
  typedef std::vector<numeric::vector2<_Tp>> pt_set_type;
  typedef typename pt_set_type::iterator iterator;
  typedef typename pt_set_type::const_iterator const_iterator;

  static const char *_svg_style; /**< SVG style */

  /** Create a empty zonotope with n elements */
  inline explicit Zonotope(size_t n)
      : _center(_Tp(0), _Tp(0)), _pt_set(n, _center) {}

  /** Create a zonotope by a vector of points */
  inline Zonotope(const Point<_Tp> &p, pt_set_type v)
      : _center(p), _pt_set(std::move(v)) {
    normalize();
  }

  /** Create a zonotope by Point<AAF> */
  template <typename _Up> Zonotope(const Point<_Up> &p);

  /** Copy constructor */
  template <typename _Up>
  inline Zonotope(const Zonotope<_Up> &cpy)
      : _center(cpy.center()), _pt_set(cpy.get_pt_set().size()) {
    std::copy(cpy.get_pt_set().begin(), cpy.get_pt_set().end(),
              _pt_set.begin());
  }

  // Compiler generated copy constructor and operator=() are fine.

  /** @return the number of vertices. */
  inline size_t num_vertices() const { return _pt_set.size() * 2; }

  /** @return the center point */
  inline const Point<_Tp> &center() const { return _center; }

  /** @return the center point */
  inline Point<_Tp> &center() { return _center; }

  /** @return signed area. (not implemented yet)
      Preconditon: zonotope is in canonical form.
      Time complexity: O(n). */
  inline _Tp area() const;

  /** @return the bounding box of this zonotope. Time complexity: O(n) */
  Rectangle<_Tp> bounding_box() const {
    _Tp t_dev_x = _Tp(0); // total derivative x
    _Tp t_dev_y = _Tp(0); // total derivative y

    for (const_iterator it = _pt_set.begin(); it != _pt_set.end(); ++it) {
      t_dev_x += (it->_a1 < 0) ? -it->_a1 : it->_a1;
      t_dev_y += it->_a2;
    }
    const numeric::interval<_Tp> x_bound(-t_dev_x, t_dev_x);
    const numeric::interval<_Tp> y_bound(-t_dev_y, t_dev_y);
    return Rectangle<_Tp>(x_bound, y_bound) + center();
  }

  /** Create a zontope with random size at random location. */
  static _Self random();

  /** Covert to a (convex) polygon representation. (not implemented yet) */
  Polygon<_Tp> convert_polygon() const;

  /**
   * @name Geometry transformations
   */
  //@{

  /** Translate this point_set by (delta_x, delta_y). */
  inline void translate(int delta_x, int delta_y) {
    _center.translate(delta_x, delta_y);
  }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() { center().rotate180(); }
  //@}

  /** @name Arithmetic */
  //@{
  /** Minkowski sum with a point p and assign.
      Time complexity: O(1). */
  inline _Self &operator+=(const Point<_Tp> &p) {
    _center += p;
    return *this;
  }

  /** Minkowski difference with a point p and assign.
      Time complexity: O(1). */
  inline _Self &operator-=(const Point<_Tp> &p) {
    _center -= p;
    return *this;
  }

  //@}

  /** @name Comparison and Relations */
  //@{

  /** Test for equality. Assume two zonotopes are in canonical form. */
  inline bool operator==(const Zonotope<_Tp> &p) const {
    return center() == p.center() && _pt_set == p._pt_set;
  }

  /** @return whether the point q is inside this zonotope.
      (not implemented yet) */
  bool contain(const Point<_Tp> &q) const;

  /** @return whether the zonotope z overlaps with this zonotope
      (not implemented yet) */
  bool overlap(const Zonotope<_Tp> &z) const;

  /** Convert a zonotope in canonical form:
      i.e. 0 <= phi1 < phi2 < ... < phiN */
  void normalize() {
    auto it = _pt_set.begin();
    for (; it != _pt_set.end(); ++it) {
      if (it->_a2 < _Tp(0))
        *it = -(*it);
    }
    std::sort(_pt_set.begin(), _pt_set.end(), Compare_By_Angle<_Tp>());
  }

  /** @return whether a zonotope is in canonical form:
      i.e. 0 <= phi1 < phi2 < ... < phiN */
  bool is_normal() const {
    const_iterator it;
    for (it = _pt_set.begin(); it != _pt_set.end(); ++it) {
      if (it->_a2 < _Tp(0))
        return false;
    }
    auto it2 = _pt_set.begin();
    Compare_By_Angle<_Tp> check;
    for (it = _pt_set.begin() + 1; it != _pt_set.end(); ++it, ++it2) {
      if (check(*it, *it2))
        return false;
    }
    return true;
  }

  /** Approximate a given zonotope w by means of zonotopes from the
      class of this zonotope (z), so that w is an outer
      approximation of z, i.e. w \subset z */
  _Self approximate(const _Self &w) const;

  // xxx /** @return the center */
  // xxx const Point<_Tp>& get_center() const { return _center; }

  /** @return the point set */
  const pt_set_type &get_pt_set() const { return _pt_set; }

private:
  Point<_Tp> _center;  /**< center */
  pt_set_type _pt_set; /**< represent basis of centered segments */
};

/** Minkowski sum with a point. Time complexity: O(1).
    @see operator+=(). */
template <typename _Tp>
inline Zonotope<_Tp> operator+(Zonotope<_Tp> z, const Point<_Tp> &p) {
  return z += p;
}

/** Minkowski difference with a point. Time complexity: O(1).
    @see operator-=(). */
template <typename _Tp>
inline Zonotope<_Tp> operator-(Zonotope<_Tp> z, const Point<_Tp> &p) {
  return z -= p;
}

/** Test for inequality. @see operator==() */
template <typename _Tp, typename _Up>
inline bool operator!=(const Zonotope<_Tp> &lhs, const Zonotope<_Up> &rhs) {
  return !(lhs == rhs);
}

/** @} */
}

#endif // RECTI_ZONOTOPE_HPP
