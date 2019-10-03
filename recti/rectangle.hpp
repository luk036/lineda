#ifndef RECTI_RECTANGLE_HPP
#define RECTI_RECTANGLE_HPP

#ifndef RECTI_POINT_HPP
#include "point.hpp"
#endif
#ifndef NUMERIC_INTERVAL_HPP
#include "interval.hpp"
#endif
#include <cassert>
#include <iosfwd> // for declaration of std::istream and std::ostream

/** Forward declarations */
class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/** Forward declarations */
template <typename _Tp> class Polygon;
template <typename _Tp> class Path;
template <typename _Tp> class Rectangle;

/** Default integer coordinate */
typedef Rectangle<int> rectangle;

/**
 * Rectangle Class. An axis-parallel rectangle.
 */
template <typename _Tp> class Rectangle : public Point<numeric::interval<_Tp>> {
  typedef Point<numeric::interval<_Tp>> _Base;

public:
  static const Rectangle<_Tp> _zero; /**< zero rectangle */
  static const char *_svg_style;     /**< svg style */

  /** @name Constructions
   */
  //@{

  /** Create a "zero" Rectangle at origin. */
  inline Rectangle() : _Base() {}

  /** Create a rectangle with x bounds and y bounds. */
  template <typename _Up>
  inline Rectangle(const numeric::interval<_Up> &xb,
                   const numeric::interval<_Up> &yb)
      : _Base(xb, yb) {}

  /** Create a rectangle by lower left point and upper right point. */
  inline Rectangle(const Point<_Tp> &ll, const Point<_Tp> &ur)
      : _Base(numeric::interval<_Tp>(ll.x(), ur.x()),
              numeric::interval<_Tp>(ll.y(), ur.y())) {}

  /** Create a rectangle with lower left point, width and height.
      Precondition: width and height are non-negative. */
  template <typename _Up>
  inline Rectangle(const Point<_Up> &p, const _Up &width, const _Up &height);

  /** Create a rectangle by another rectangle. */
  template <typename _Up>
  inline Rectangle(const Rectangle<_Up> &r) : _Base(r) {}

  /** Create a rectangle by the base class. */
  template <typename _Up>
  inline Rectangle(const Point<numeric::interval<_Up>> &r) : _Base(r) {}

  /** Create a Rectangle with random size at random location. */
  inline static Rectangle<_Tp> random() {
    return static_cast<Rectangle<_Tp>>(_Base::random());
  }

  //@}

  // Compiler generated copy constructor and operator=() are fine.

  /** @name Basic Operations
   */
  //@{
  /** @return x bound */
  inline const numeric::interval<_Tp> &x_bound() const { return _Base::x(); }

  /** @return y bound */
  inline const numeric::interval<_Tp> &y_bound() const { return _Base::y(); }

  /** @return lower left point */
  inline Point<_Tp> ll() const { return Point<_Tp>(llx(), lly()); }

  /** @return upper right point */
  inline Point<_Tp> ur() const { return Point<_Tp>(urx(), ury()); }

  /** @return lower right point */
  inline Point<_Tp> lr() const { return Point<_Tp>(urx(), lly()); }

  /** @return upper left point */
  inline Point<_Tp> ul() const { return Point<_Tp>(llx(), ury()); }

  /** @return lower left x coordinate. */
  inline const _Tp &llx() const { return x_bound().lower(); }

  /** @return lower left y coordinate. */
  inline const _Tp &lly() const { return y_bound().lower(); }

  /** @return upper right x coordinate. */
  inline const _Tp &urx() const { return x_bound().upper(); }

  /** @return upper right y coordinate. */
  inline const _Tp &ury() const { return y_bound().upper(); }

  /** @return width */
  inline _Tp width() const { return x_bound().range(); }

  /** @return height */
  inline _Tp height() const { return y_bound().range(); }

  /** @return half perimeter */
  inline _Tp half_perimeter() const { return width() + height(); }

  /** @return area */
  inline _Tp area() const { return width() * height(); }

  /** @return the bounding_box */
  inline const Rectangle<_Tp> &bounding_box() const { return *this; }
  //@}

  /** @name Geometry Transformations
   */
  //@{

  /** Translate by (delta_x, delta_y). */
  template <typename _Up>
  inline void translate(const _Up &delta_x, const _Up &delta_y) {
    _Base::translate(delta_x, delta_y);
  }

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090() { _Base::rotate090(); }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() { _Base::rotate180(); }

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270() { _Base::rotate270(); }

  /** Flip against y-axis. */
  inline void flip_y() { _Base::flip_y(); }

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y() { _Base::rotate090_then_flip_y(); }

  /** Rotate 180 degree anti-clockwise and then flip against y-axis. */
  inline void rotate180_then_flip_y() { _Base::rotate180_then_flip_y(); }

  /** Rotate 270 degree anti-clockwise and then flip against y-axis. */
  inline void rotate270_then_flip_y() { _Base::rotate270_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis. */
  inline void flip_x() { rotate180_then_flip_y(); }
  //@}

  /** @name Arithmetic */
  //@{
  /** Minkowski sum with a point p and assign */
  template <typename _Up>
  inline Rectangle<_Tp> &operator+=(const Point<_Up> &p) {
    _Base::operator+=(p);
    return *this;
  }

  /** Minkowski difference with a point p and assign */
  template <typename _Up>
  inline Rectangle<_Tp> &operator-=(const Point<_Up> &p) {
    _Base::operator-=(p);
    return *this;
  }

  /** Minkowski sum and assign */
  template <typename _Up>
  inline Rectangle<_Tp> &operator+=(const Rectangle<_Up> &r) {
    _Base::operator+=(r);
    return *this;
  }

  /** Minkowski difference and assign */
  template <typename _Up>
  inline Rectangle<_Tp> &operator-=(const Rectangle<_Up> &r) {
    _Base::operator-=(r);
    return *this;
  }
  //@}

  /** @name Comparison and Relations */
  //@{

  /** @return whether it is below r */
  template <typename _Up> inline bool below(const Rectangle<_Up> &r) const {
    return _Base::below(r);
  }

  /** @return whether it is above r */
  template <typename _Up> inline bool above(const Rectangle<_Up> &r) const {
    return _Base::above(r);
  }

  /** @return whether it is left of r */
  template <typename _Up> inline bool left_of(const Rectangle<_Up> &r) const {
    return _Base::left_of(r);
  }

  /** @return whether it is right of r */
  template <typename _Up> inline bool right_of(const Rectangle<_Up> &r) const {
    return _Base::right_of(r);
  }

  /** @name Inclusion and Intersection */
  //@{
  /** @return whether it overlaps r.
      Note that this relation is reflexive, symmetry but not transitve. */
  template <typename _Up> inline bool overlap(const Rectangle<_Up> &r) const {
    return x_bound().overlap(r.x_bound()) && y_bound().overlap(r.y_bound());
  }

  /** @return whether the point q is inside this rectangle. */
  template <typename _Up> inline bool contain(const Point<_Up> &q) const {
    return x_bound().contain(q.x()) && y_bound().contain(q.y());
  }

  /** @return whether the rectangle r is completely inside this rectangle. */
  template <typename _Up> inline bool contain(const Rectangle<_Up> &r) const {
    return x_bound().contain(r.x_bound()) && y_bound().contain(r.y_bound());
  }

  /** @return intersection of this rectangle and r.
      Precondition: two rectangles are overlapped. */
  inline Rectangle<_Tp> intersect(const Rectangle<_Tp> &r) const {
    return Rectangle<_Tp>(x_bound().intersect(r.x_bound()),
                          y_bound().intersect(r.y_bound()));
  }

  /** @return the bounding box of this rectangle and r. */
  inline Rectangle<_Tp> hull(const Rectangle<_Tp> &r) const {
    return Rectangle<_Tp>(x_bound().hull(r.x_bound()),
                          y_bound().hull(r.y_bound()));
  }
  //@}
};

/** @return an interval given two numbers */
template <typename _Tp>
inline Rectangle<_Tp> make_rectangle(Point<_Tp> p1, Point<_Tp> p2) {
  return Rectangle<_Tp>(numeric::make_interval<_Tp>(p1.x(), p2.x()),
                        numeric::make_interval<_Tp>(p1.y(), p2.y()));
}

/** Create a rectangle with lower left point, width and height.
    Precondition: width and height are non-negative. */
template <typename _Tp>
template <typename _Up>
inline Rectangle<_Tp>::Rectangle(const Point<_Up> &ll, const _Up &width,
                                 const _Up &height)
    : _Base(numeric::interval<_Tp>(ll.x(), ll.x() + width),
            numeric::interval<_Tp>(ll.y(), ll.y() + height)) {
  assert(!(width < 0 || height < 0));
}

// Non member functions

/** Minkowski sum with a point */
template <typename _Tp>
inline Rectangle<_Tp> operator+(Rectangle<_Tp> R, const Point<_Tp> &p) {
  return R += p;
}

/** Minkowski difference with a point */
template <typename _Tp>
inline Rectangle<_Tp> operator-(Rectangle<_Tp> R, const Point<_Tp> &p) {
  return R -= p;
}

/** Minkowski sum. This operation is commutative and associative. */
template <typename _Tp>
inline Rectangle<_Tp> operator+(Rectangle<_Tp> l, const Rectangle<_Tp> &r) {
  return l += r;
}

/** Minkowski diff. This operation is commutative and associative. */
template <typename _Tp>
inline Rectangle<_Tp> operator-(Rectangle<_Tp> l, const Rectangle<_Tp> &r) {
  return l -= r;
}

/** Print the rectangle in plain text format. The reason this function is
    not inlined is that I don't want the iostream header file
    included in any header files. */
template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const Rectangle<_Tp> &r);

/** Print the rectangle in SVG format. The reason this function is
    not inlined is that I don't want the iostream header file
    included in any header files. */
template <typename _Tp>
svgstream &operator<<(svgstream &osvgs, const Rectangle<_Tp> &r);

/** @} */
}

#endif // RECTI_RECTANGLE_HPP
