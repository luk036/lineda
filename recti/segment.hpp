#ifndef RECTI_SEGMENT_HPP
#define RECTI_SEGMENT_HPP

#include <algorithm>
#include <cassert>
#include <cstdlib> // for abs()
#include <iosfwd>  // for declaration of std::istream and std::ostream
#include <numericfwd.hpp>

#include "point.hpp"
#include <vector2.hpp>

/** Forward declarations */
class svgstream;

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/** Forward declarations */
template <typename _Tp> class Segment;
template <typename _Tp> class Rectangle;

/** Default integer coordinate */
typedef Segment<int> segment;

/**
 * Segment Class. A 2D segment. line segment holds two points
 * (called start and end points).
 */
template <typename _Tp> class Segment {
  typedef Segment<_Tp> _Self;

public:
  static const Segment<_Tp> _origin; /**< Segment (0,0) */
  static const char *_svg_style;     /**< SVG style */
  static _Tp _svg_radius;            /**< SVG radius */

  /** @name Constructions
   */
  //@{
  /** Create a segment at origin. */
  inline Segment() : _start(), _end() {}

  /** Create a segment at (start, end). */
  inline Segment(const Point<_Tp> &start, const Point<_Tp> &end)
      : _start(start), _end(end) {}

  // Compiler generated copy constructor and operator= are fine

  /** Create a segment by another segment */
  template <typename _Up>
  Segment(const Segment<_Up> &l) : _start(l.start()), _end(l.end()) {}

  /** Create a segment at random location */
  inline static Segment<_Tp> random() {
    return Segment<_Tp>(Point<_Tp>::random(), Point<_Tp>::random());
  }
  //@}

  /** @name Basic Operations
   */
  //@{
  /** @return start point */
  inline const Point<_Tp> &start() const { return _start; }

  /** @return end point */
  inline const Point<_Tp> &end() const { return _end; }

  /** @return start point */
  inline Point<_Tp> &start() { return _start; }

  /** @return end point */
  inline Point<_Tp> &end() { return _end; }

  /** @return whether this segment is a horizontal line */
  inline bool is_horizontal() const { return start().is_horizontal(end()); }

  /** @return whether this segment is a vertical line */
  inline bool is_vertical() const { return start().is_vertical(end()); }

  /** @return the bounding_box.
      Note that you may need to include "rectanlge.hpp" in your code */
  inline Rectangle<_Tp> bounding_box() const {
    return Rectangle<_Tp>(numeric::make_interval(start().x(), end().x()),
                          numeric::make_interval(start().y(), end().y()));
  }

  /** @return area (i.e. 0) */
  inline _Tp area() const { return _Tp(); };

  /**
   * @name Geometry Transformations
   */
  //@{

  /** Translate by (delta_x, delta_y). */
  template <typename _Up>
  inline void translate(const _Up &delta_x, const _Up &delta_y) {
    parallel(_start += delta_x, _end += delta_y);
  }

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090() { start().rotate090(), end().rotate090(); }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() { start().rotate180(), end().rotate180(); }

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270() { start().rotate270(), end().rotate270(); }

  /** Flip against y-axis */
  inline void flip_y() { start().flip_y(), end().flip_y(); }

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y() {
    start().rotate090_then_flip_y(), end().rotate090_then_flip_y();
  }

  /** Rotate 180 degree anti-clockwise and then flip against y-axis */
  inline void rotate180_then_flip_y() {
    start().rotate180_then_flip_y(), end().rotate180_then_flip_y();
  }

  /** Rotate 270 degree anti-clockwise and then flip against y-axis */
  inline void rotate270_then_flip_y() {
    start().rotate270_then_flip_y(), end().rotate270_then_flip_y();
  }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }

  /** Reverse the direction */
  inline void reverse() { std::swap(start(), end()); }

  //@}

  /**
   * @name Arithmetic
   */
  //@{
  /** Minkowski sum and assign */
  template <typename _Up> inline Segment<_Tp> &operator+=(const Point<_Up> &q) {
    parallel(_start += q, _end += q);
    return *this;
  }

  /** Minkowski difference and assign */
  template <typename _Up> inline Segment<_Tp> &operator-=(const Point<_Up> &q) {
    parallel(_start -= q, _end -= q);
    return *this;
  }

  /** Multiply and assign */
  template <typename _Up> inline Segment<_Tp> &operator*=(const _Up &a) {
    parallel(_start *= a, _end *= a);
    return *this;
  }

  /** Divide and assign */
  template <typename _Up> inline Segment<_Tp> &operator/=(const _Up &a) {
    parallel(_start /= a, _end /= a);
    return *this;
  }

  //@}

  /**
   * @name Comparisons and Relations
   */
  //@{
  /** @return whether q left of this line segment ??? */
  template <typename _Up> inline bool left(const Point<_Up> &q) const {
    return area2(start(), end(), q) > 0;
  }

  /** @return whether q is right of this line segment ??? */
  template <typename _Up> inline bool right(const Point<_Up> &q) const {
    return area2(start(), end(), q) < 0;
  }

  /** @return whether it is left of a line segment l. ???
      Note that this relation is irreflexive */
  template <typename _Up> inline bool left_of(const Segment<_Up> &l) const {
    return l.left(start()) && l.left(end());
  }

  /** @return whether it is right of a line segment l ???
      Note that this relation is irreflexive */
  template <typename _Up> inline bool right_of(const Segment<_Up> &l) const {
    return l.right(start()) && l.right(end());
  }

  //@}

  /** @name Inclusion and Intersection */
  //@{
  /** @return whether a line segment s straddles the line extended
   * by this segment. A segment (p3,p4) straddles a line (p1, p2) if
   * point p3 lies on one side of the line and point p4 lies on the
   * other side, or p3 or p4 lies on the line.
   *
   * Reference: Introduction to Algorithms (M.I.T. press), Chapter 35
   */
  bool straddle(const Segment<_Tp> &s) const {
    const numeric::vector2<_Tp> p = end() - start();
    const _Tp c1 = det(p, s.start() - start());
    const _Tp c2 = det(p, s.end() - start());
    return !((c1 > 0 && c2 > 0) || (c1 < 0 && c2 < 0));
  }

  /** @return whether this line segment overlaps (intersects) s.
   *  Note that you may need to include "rectanlge.hpp" in your code.
   *
   * Reference: Introduction to Algorithms (M.I.T. press), Chapter 35
   */
  template <typename _Up> bool overlap(const Segment<_Up> &s) const {
    // The line segments cannot intersect if their bounding boxes do
    // not interest.
    if (!bounding_box().overlap(s.bounding_box()))
      return false;
    return straddle(s) && s.straddle(*this);
  }

  /** @return whether a line extended by this segment straddles a
   * rectangle r.
   */
  template <typename _Up> bool straddle(const Rectangle<_Up> &r) const {
    return straddle(Segment<_Up>(r.ll(), r.ur())) ||
           straddle(Segment<_Up>(r.ul(), r.lr()));
  }

  /** @return whether it overlaps a rectangle r.
   *  Note that you may need to include "rectanlge.hpp" in your code.
   */
  template <typename _Up> bool overlap(const Rectangle<_Up> &r) const {
    return bounding_box().overlap(r) && straddle(r);
  }

  //@}
private:
  // Just indicate that two operations can be executed in parallel
  template <class _Up1, class _Up2>
  inline void parallel(const _Up1 &, const _Up2 &) const {}

private:
  Point<_Tp> _start; /**< start point */
  Point<_Tp> _end;   /**< end point */
};

/** @} */

// Non-member function declaration

/** Minkowski sum with a point q */
template <typename _Tp>
inline Segment<_Tp> operator+(Segment<_Tp> S, const Point<_Tp> &q) {
  return S += q;
}

/** Minkowski difference with a point q */
template <typename _Tp>
inline Segment<_Tp> operator-(Segment<_Tp> S, const Point<_Tp> &q) {
  return S -= q;
}

/** Multiply (scaling) */
template <typename _Tp>
inline Segment<_Tp> operator*(Segment<_Tp> S, const _Tp &a) {
  return S *= a;
}

/** Divide */
template <typename _Tp>
inline Segment<_Tp> operator/(Segment<_Tp> S, const _Tp &a) {
  return S /= a;
}

/** @return whether it is equal to s. */
template <typename _Tp, typename _Up>
inline bool operator==(const Segment<_Tp> lhs, const Segment<_Up> &rhs) {
  return lhs.start() == rhs.start() && lhs.end() == rhs.end();
}

/** @return whether it is not equal to s. */
template <typename _Tp, typename _Up>
inline bool operator!=(const Segment<_Tp> lhs, const Segment<_Up> &rhs) {
  return !(lhs == rhs);
}

/* Print the segment in plain text form. The reason this function is
    not inlined is that I don't want the iostream header file
    included in any header files. */
template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const Segment<_Tp> &p);

/** Print the segment in SVG format.*/
template <typename _Tp>
svgstream &operator<<(svgstream &osvgs, const Segment<_Tp> &p);
}

#endif // RECTI_SEGMENT_HPP
