#ifndef RECTI_POINT_HPP
#define RECTI_POINT_HPP

#include <boost/operators.hpp>
#include <cmath>   // for fabs()
#include <cstdlib> // for abs()
#include <iosfwd>  // for declaration of std::istream and std::ostream
#include <vector2.hpp>

/** Forward declarations */
class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/** Forward declarations */
template <typename _Tp> class Point;

/**
 * Point Class. A 2D point. Point holds two objects
 * (called x- and y-coordinates) of same numerical type.
 */
template <typename _Tp = int>
class Point : boost::additive<Point<_Tp>, numeric::vector2<_Tp>> {
  typedef Point<_Tp> _Self;

public:
  typedef _Tp value_type;

  static const Point<_Tp> _origin; /**< Point (0,0) */
  static const char *_svg_style;   /**< SVG style */
  static int _svg_radius;          /**< SVG radius */

  /** @name Constructions
   */
  //@{
  /** Create a point at origin. */
  inline Point() : _x(_Tp()), _y(_Tp()) {}

  /** Create a point at (x, y). */
  inline Point(const _Tp &x, const _Tp &y) : _x(x), _y(y) {}

  // Compiler generated copy constructor and operator= are fine

  /** Create a point by another point */
  template <typename _Up> Point(const Point<_Up> &p) : _x(p.x()), _y(p.y()) {}

  /** Create a point at random location */
  inline static _Self random() { return _Self(_Tp::random(), _Tp::random()); }

  //@}

  /** @name Basic Operations
   */
  //@{
  /** @return x coordinate */
  inline const _Tp &x() const { return _x; }

  /** @return y coordinate */
  inline const _Tp &y() const { return _y; }

  /** @return area (i.e. 0) */
  inline _Tp area() const { return _Tp(); };

  /**
   * @name Geometry Transformations
   */
  //@{

  /** Translate by (delta_x, delta_y). */
  template <typename _Up>
  inline void translate(const _Up &delta_x, const _Up &delta_y) {
    _x += delta_x;
    _y += delta_y;
  }

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090() {
    const _Tp temp = _x;
    _x = -_y;
    _y = temp;
  }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() {
    _x = -_x;
    _y = -_y;
  }

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270() {
    const _Tp temp = _y;
    _y = -_x;
    _x = temp;
  }

  /** Flip against y-axis */
  inline void flip_y() { _x = -_x; }

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y() {
    const _Tp temp = _x;
    _x = _y;
    _y = temp;
  }

  /** Rotate 180 degree anti-clockwise and then flip against y-axis */
  inline void rotate180_then_flip_y() { _y = -_y; }

  /** Rotate 270 degree anti-clockwise and then flip against y-axis */
  inline void rotate270_then_flip_y() {
    const _Tp temp = _y;
    _y = -_x;
    _x = -temp;
  }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }
  //@}

  /**
   * @name Arithmetic
   */
  //@{
  /** Plus and assign. Minkowski sum and assign */
  inline Point<_Tp> &operator+=(const Point<_Tp> &q) {
    _x += q.x();
    _y += q.y();
    return *this;
  }

  /** Plus a vector and assign. */
  inline Point<_Tp> &operator+=(const numeric::vector2<_Tp> &v) {
    _x += v._a1;
    _y += v._a2;
    return *this;
  }

  /** Substract a vector and assign. */
  inline Point<_Tp> &operator-=(const numeric::vector2<_Tp> &v) {
    _x -= v._a1;
    _y -= v._a2;
    return *this;
  }

  // xxx /** Subtract (Minkowski difference) */
  // xxx inline Point<_Tp>& operator-=(const numeric::vector2<_Tp>& q)
  // xxx { _x -= q.x(); _y -= q.y(); return *this; }

  /** Multiply and assign */
  template <typename _Up> inline Point<_Tp> &operator*=(const _Up &a) {
    _x *= a;
    _y *= a;
    return *this;
  }

  /** Divide and assign */
  template <typename _Up> inline Point<_Tp> &operator/=(const _Up &a) {
    _x /= a;
    _y /= a;
    return *this;
  }

  /** Cross product */
  // xxx _Tp cross(const Point<_Tp>& q) const
  // xxx { return x() * q.y() - q.x() * y(); }

  //@}

  /**
   * @name Comparisons and Relations
   */
  //@{
  /** @return whether it is below q */
  template <typename _Up> inline bool below(const Point<_Up> &q) const {
    return _y < q._y;
  }

  /** @return whether it is above q */
  template <typename _Up> inline bool above(const Point<_Up> &q) const {
    return _y > q._y;
  }

  /** @return whether it is left of q */
  template <typename _Up> inline bool left_of(const Point<_Up> &q) const {
    return _x < q._x;
  }

  /** @return whether it is right of q */
  template <typename _Up> inline bool right_of(const Point<_Up> &q) const {
    return _x > q._x;
  }

  /** @return whether two points are on a horizontal line */
  template <typename _Up> inline bool is_horizontal(const Point<_Up> &q) const {
    return _y == q._y;
  }

  /** @return whether two points are on a vertical line */
  template <typename _Up> inline bool is_vertical(const Point<_Up> &q) const {
    return _x == q._x;
  }
  //@}

private:
  _Tp _x; /**< x coordinate */
  _Tp _y; /**< y coordinate */
};

/** @} */

/** Create a point at random location */
template <> inline Point<int> Point<int>::random() {
  return Point<int>((rand() - rand()) % 1000, (rand() - rand()) % 1000);
}

// Non-member inline functions

/** @return the Manhattan distance of two points */
template <typename _Tp>
inline _Tp distance(const Point<_Tp> &p, const Point<_Tp> &q) {
  return distance(p.x(), q.x()) + distance(p.y(), q.y());
}

/** @return the Manhattan distance of two points */
template <>
inline double distance(const Point<double> &p, const Point<double> &q) {
  return fabs(p.x() - q.x()) + fabs(p.y() - q.y());
}

/** @return the Manhattan distance of two points */
template <> inline int distance(const Point<int> &p, const Point<int> &q) {
  return abs(p.x() - q.x()) + abs(p.y() - q.y());
}

/** @return the cross product. Note: p x q = - q x p */
template <typename _Tp>
inline _Tp cross_product(const Point<_Tp> &p, const Point<_Tp> &q) {
  return p.x() * q.y() - q.x() * p.y();
}

/** @return 2x Signed area of a triangle given by three points. */
template <typename _Tp>
inline _Tp area2(const Point<_Tp> &p, const Point<_Tp> &q,
                 const Point<_Tp> &r) {
  return numeric::det(q - p, r - p);
}

// Non-member function declaration

/** @return whether it is equal to q. */
template <typename _Tp, typename _Up>
inline bool operator==(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return lhs.x() == rhs.x() && lhs.y() == rhs.y();
}

/** @return whether it is less than q. A point p is less
    than q if p is left of q. If two points are in vertical,
    then p is less than q if p is below q. In other words,
    x coordinate has higher priority. */
template <typename _Tp, typename _Up>
inline bool operator<(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return lhs.x() < rhs.x() || (!(rhs.x() < lhs.x()) && lhs.y() < rhs.y());
}

/** Multiply */
template <typename _Tp, typename _Up>
inline Point<_Tp> operator*(Point<_Tp> lhs, const _Up &rhs) {
  return lhs *= rhs;
}

/** Divide */
template <typename _Tp, typename _Up>
inline Point<_Tp> operator/(Point<_Tp> lhs, const _Up &rhs) {
  return lhs /= rhs;
}

/** Plus (Minkowski sum). This operation is commutative and associative. */
template <typename _Tp>
inline Point<_Tp> operator+(Point<_Tp> lhs, const Point<_Tp> &rhs) {
  return lhs += rhs;
}

// xxx /** Subtract (Minkowski difference). */
// xxx template <typename _Tp>
// xxx inline Point<_Tp> operator-(Point<_Tp> lhs, const Point<_Tp>& rhs)
// xxx { return lhs -= rhs; }

/** Subtract */
template <typename _Tp>
inline numeric::vector2<_Tp> operator-(Point<_Tp> lhs, const Point<_Tp> &rhs) {
  return numeric::vector2<_Tp>(lhs.x() - rhs.x(), lhs.y() - rhs.y());
}

/** @return whether it is not equal to q. */
template <typename _Tp, typename _Up>
inline bool operator!=(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return !(lhs == rhs);
}

/** Greater than or equal to. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator>=(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return !(lhs < rhs);
}

/** Less than or equal to. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator<=(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return !(rhs < lhs);
}

/** Greater than. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator>(const Point<_Tp> &lhs, const Point<_Up> &rhs) {
  return rhs < lhs;
}

/** Print the point in plain text form. The reason this function is
    not inlined is that I don't want the iostream header file
    included in any header files. */
template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const Point<_Tp> &p) {
  os << "(" << p.x() << ", " << p.y() << ")";
  return os;
}

/** Print the point in SVG format.*/
template <typename _Tp>
svgstream &operator<<(svgstream &osvgs, const Point<_Tp> &p);

/** Dual Point */
template <typename _Tp> class duPoint : public Point<_Tp> {
  typedef Point<_Tp> Base;

public:
  duPoint(const Base &p) : Base(p) {}
  ~duPoint() = default;
  const _Tp &x() const { return y(); }
  _Tp &x() { return y(); }
  const _Tp &y() const { return x(); }
  _Tp &y() { return x(); }
};

/** Default integer coordinate */
typedef Point<int> point;
}

#endif // RECTI_POINT_HPP
