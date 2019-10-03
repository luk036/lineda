#ifndef RECTI_POINT_SET_HPP
#define RECTI_POINT_SET_HPP

#include "point.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <iostream>
#include <utility>
#include <vector>

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/* Forward declaration. */
template <typename _Tp> class Point_set;
template <typename _Tp> class Rectangle;

typedef Point_set<int> point_set;

/**
 * Point Set Class. Implemented by a vector of points
 * [p0, p1, p2, ... pN-1]. Based class of path and polygon.
 */
template <typename _Tp> class Point_set {
  typedef std::vector<Point<_Tp>> pt_vec_type;

public:
  /** Default constructor */
  inline Point_set() : _pt_vec() {}

  /** Create a Point_set by a vector of points */
  inline Point_set(size_t n) : _pt_vec(n) {}

  /** Create a Point_set by a vector of points */
  inline Point_set(pt_vec_type v) : _pt_vec(std::move(v)) {}

  /** Create a Point_set by an array of points. */
  inline Point_set(const Point<_Tp> *a, size_t s) : _pt_vec(a, a + s) {}

  // Default copy constructor and assignment are fine.

  /** @return the number of vertices. */
  inline size_t num_vertices() const { return _pt_vec.size(); }

  /** @return the bounding box of this point set. */
  Rectangle<_Tp> bounding_box() const;

  /**
   * @name Geometry transformations
   */
  //@{

  /** Translate this Point_set by (delta_x, delta_y). */
  inline void translate(int delta_x, int delta_y);

  /** Rotate 90 degree anti-clockwise. */
  void rotate090();

  /** Rotate 180 degree anti-clockwise. */
  void rotate180();

  /** Rotate 270 degree anti-clockwise. */
  void rotate270();

  /** Flip against y-axis. */
  void flip_y();

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  void rotate090_then_flip_y();

  /** Rotate 180 degree anti-clockwise and then flip against y-axis. */
  void rotate180_then_flip_y();

  /** Rotate 270 degree anti-clockwise and then flip against y-axis. */
  void rotate270_then_flip_y();

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }
  //@}

  /** Minkowski sum with a point q and assign */
  template <typename _Up> Point_set<_Tp> &operator+=(const Point<_Up> &q);

  /** Minkowski difference with a point q and assign */
  template <typename _Up> Point_set<_Tp> &operator-=(const Point<_Up> &q);

  // Comparison operators

  /** Test for equality. Assume two Point_sets are in same order. */
  template <typename _Up>
  inline bool operator==(const Point_set<_Up> &p) const {
    return _pt_vec == p._pt_vec;
  }

public:
  pt_vec_type _pt_vec; /**< vector of points */
};

/** @} */

// Non-member operators and functions

/** Minkowski sum with a point q. */
template <typename _Tp, typename _Up>
inline Point_set<_Tp> operator+(Point_set<_Tp> P, const Point<_Up> &q) {
  return P += q;
}

/** Minkowski difference with a point q. */
template <typename _Tp, typename _Up>
inline Point_set<_Tp> operator-(Point_set<_Tp> P, const Point<_Up> &q) {
  return P -= q;
}

/** Test for inequality. @see operator==() */
template <typename _Tp, typename _Up>
inline bool operator!=(const Point_set<_Tp> &p, const Point_set<_Up> &q) {
  return !(p == q);
}
}

#endif // RECTI_POINT_SET_HPP
