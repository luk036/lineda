#include <utility>

#ifndef RECTI_CHECKER_HPP
#define RECTI_CHECKER_HPP

/**
 * @defgroup recti Rectilinear Shapes Module
 * @ingroup lineda
 *
 * This module provides basic geometric objects such as point,
 * rectangle, polygon, path, pattern, Manhattan arc, etc. Each class
 * provides operations such as rotations, flippings, coordinate
 * transformations, overlap detection with other classes, etc.
 * Note that don't directly add data members to the classes because
 * there could be multi-millions of such objects. Instead, use
 * inheritance to add responsiblitiesin your classes. Finally, this
 * module should be on top of the dsl module, e.g., polygon may be
 * implemented by circular linked list of points.
 */

namespace recti {
/**
 * @addtogroup recti
 * @{
 */

/** Generic Checker. */
template <class _Tp> class checker {
public:
  /** Constructor */
  inline checker(_Tp a) : _a(std::move(a)) {}

  /** Check if rotate090 function is OK */
  inline bool rotate090_ok() const;

  /** Check if rotate180 function is OK */
  inline bool rotate180_ok() const;

  /** Check if rotate270 function is OK */
  inline bool rotate270_ok() const;

  /** Check if flip_y function is OK */
  inline bool flip_y_ok() const;

  /** Check if rotate090_then_flip_y function is OK */
  inline bool rotate090_then_flip_y_ok() const;

  /** Check if rotate180_then_flip_y function is OK */
  inline bool rotate180_then_flip_y_ok() const;

  /** Check if rotate270_then_flip_y function is OK */
  inline bool rotate270_then_flip_y_ok() const;

  /** Check if rotate functions are OK */
  inline bool rotate_ok() const;

  /** Check if rotate_then_flip_y functions are OK */
  inline bool rotate_then_flip_ok() const;

private:
  _Tp _a; /**< the test case object to be checked */
};

/** @return whether rotate090() is verified OK for a test case based
    on the rotational symmetric. */
template <class _Tp> inline bool checker<_Tp>::rotate090_ok() const {
  _Tp b(_a);
  b.rotate090();
  b.rotate090();
  b.rotate090();
  b.rotate090();
  return _a == b;
}

/** @return whether rotate180() is verified OK for a test case based
    on the rotational symmetric. */
template <class _Tp> inline bool checker<_Tp>::rotate180_ok() const {
  _Tp b(_a);
  b.rotate180();
  b.rotate180();
  return _a == b;
}

/** @return whether rotate270() is verified OK for a test case based
    on the rotational symmetric. */
template <class _Tp> inline bool checker<_Tp>::rotate270_ok() const {
  _Tp b(_a);
  b.rotate270();
  b.rotate270();
  b.rotate270();
  b.rotate270();
  return _a == b;
}

/** @return whether flip_y() is verified OK for a test case based on
    the reflexive symmetric. */
template <class _Tp> inline bool checker<_Tp>::flip_y_ok() const {
  _Tp b(_a);
  b.flip_y();
  b.flip_y();
  return _a == b;
}

/** @return whether rotate090_then_flip_y() is verified OK for a test
    case based on the rotational symmetric. */
template <class _Tp>
inline bool checker<_Tp>::rotate090_then_flip_y_ok() const {
  _Tp b(_a);
  b.rotate090_then_flip_y();
  b.rotate090_then_flip_y();
  return _a == b;
}

/** @return whether rotate180_then_flip_y() is verified OK for a
    test case based on the rotational symmetric. */
template <class _Tp>
inline bool checker<_Tp>::rotate180_then_flip_y_ok() const {
  _Tp b(_a);
  b.rotate180_then_flip_y();
  b.rotate180_then_flip_y();
  return _a == b;
}

/** @return whether rotate270_then_flip_y() is verified OK for a
    test case based on the rotational symmetric. */
template <class _Tp>
inline bool checker<_Tp>::rotate270_then_flip_y_ok() const {
  _Tp b(_a);
  b.rotate270_then_flip_y();
  b.rotate270_then_flip_y();
  return _a == b;
}

/** @return whether rotation functions are verified OK for a test
    case based on the rotational symmetric. */
template <class _Tp> inline bool checker<_Tp>::rotate_ok() const {
  _Tp b(_a);
  b.rotate090();
  b.rotate180();
  b.rotate270();
  b.rotate180();
  return _a == b;
}

/** @return whether rotate_then_flip functions are verified OK for a
    test case based on the symmetric. */
template <class _Tp> inline bool checker<_Tp>::rotate_then_flip_ok() const {
  _Tp b(_a);
  b.rotate090_then_flip_y();
  b.rotate180_then_flip_y();
  b.rotate270_then_flip_y();
  b.rotate180();
  b.flip_y();
  return _a == b;
}

/** @} */
}

#endif
