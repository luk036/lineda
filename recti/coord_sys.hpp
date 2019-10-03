#ifndef RECTI_COORD_SYS_HPP
#define RECTI_COORD_SYS_HPP

#include <utility>

#include "orient.hpp"
#include "point.hpp"

namespace recti {
/**
 * @addtogroup recti
 * @{
 */
template <typename _Tp> class Coord_sys;
typedef Coord_sys<int> coord_sys;

/**
 * A 2D coordinate system.
 */
template <typename _Tp> class Coord_sys {
  typedef Coord_sys<_Tp> _Self;

public:
  static _Self _identity; /**< same as the parent coord. sys. */

  /** Default constructor. Create a coord_sys at origin. */
  inline Coord_sys() : _orient(), _origin() {}

  /** Construct a coord_sys at p. */
  inline Coord_sys(Point<_Tp> p) : _orient(), _origin(std::move(p)) {}

  // Compiler generated copy constructor and assignment operator are fine.

  /** Construct a coord_sys from another coord_sys. */
  template <typename _Up>
  inline Coord_sys(const Coord_sys<_Up> &cs)
      : _orient(cs.orientation()), _origin(cs.origin()) {}

  /** @return the origin coordinate referred to parent coord. sys. */
  inline const Point<_Tp> &origin() const { return _origin; }

  /** @return the orientation referred to the parent coord. sys. */
  inline orient orientation() const { return _orient; }

  /** @name Geometry Transformations
   */
  //@{

  /** Translate this coord_sys by (delta_x, delta_y). */
  inline void translate(int delta_x, int delta_y) {
    _origin.translate(delta_x, delta_y);
  }

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090() { _orient.rotate090(), _origin.rotate090(); }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() { _orient.rotate180(), _origin.rotate180(); }

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270() { _orient.rotate270(), _origin.rotate270(); }

  /** Flip against y-axis */
  inline void flip_y() { _orient.flip_y(), _origin.flip_y(); }

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y() {
    _orient.rotate090_then_flip_y(), _origin.rotate090_then_flip_y();
  }

  /** Rotate 180 degree anti-clockwise and then flip against y-axis */
  inline void rotate180_then_flip_y() {
    _orient.rotate180_then_flip_y(), _origin.rotate180_then_flip_y();
  }

  /** Rotate 270 degree anti-clockwise and then flip against y-axis */
  inline void rotate270_then_flip_y() {
    _orient.rotate270_then_flip_y(), _origin.rotate270_then_flip_y();
  }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }

  /** Transform the object obj to this coordinate system */
  template <class geom_obj> inline void xform_to(geom_obj &obj) const {
    obj -= _origin;
    _orient.xform_to(obj);
  }

  /** Transform the object obj from this coordinate system */
  template <class geom_obj> inline void xform_from(geom_obj &obj) const {
    _orient.xform_from(obj);
    obj += _origin;
  }

  //@}

  /** Translate via p */
  template <typename _Up>
  inline Coord_sys<_Tp> &operator+=(const Point<_Up> &p) {
    _origin += p;
    return *this;
  }

  /** Translate via p */
  template <typename _Up>
  inline Coord_sys<_Tp> &operator-=(const Point<_Up> &p) {
    _origin -= p;
    return *this;
  }

private:
  orient _orient;     /**< orientation */
  Point<_Tp> _origin; /**< origin */
};

/** Minsowki sum */
template <typename _Tp, typename _Up>
inline Coord_sys<_Tp> operator+(Coord_sys<_Tp> lhs, const Point<_Up> &rhs) {
  return lhs += rhs;
}

/** Minsowki difference */
template <typename _Tp, typename _Up>
inline Coord_sys<_Tp> operator-(Coord_sys<_Tp> lhs, const Point<_Up> &rhs) {
  return lhs -= rhs;
}

/** Equal. Two coord_sys's are equal if and only both their
    orientation and origin are same. */
template <typename _Tp, typename _Up>
inline bool operator==(const Coord_sys<_Tp> &lhs, const Coord_sys<_Up> &rhs) {
  return lhs.origin() == rhs.origin() && lhs.orientation() == rhs.orientation();
}

/** Not equal. @see operator==() */
template <typename _Tp, typename _Up>
inline bool operator!=(const Coord_sys<_Tp> &lhs, const Coord_sys<_Up> &rhs) {
  return !(lhs == rhs);
}

/** @} */
}

#endif // RECTI_COORD_SYS_HPP
