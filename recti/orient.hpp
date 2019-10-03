#ifndef RECTI_ORIENT_HPP
#define RECTI_ORIENT_HPP

#include <cstdlib> // for rand()

namespace recti {

/**
 * @addtogroup recti
 * @{
 */

/**
 * Orientation Class. A 2D orientation.
 */
class orient {
public:
  /** Orientation Enum */
  enum Orient {
    ROT000, /**< original orientation */
    ROT090, /**< rotated anti-clockwise 90 degree */
    ROT180, /**< rotated anti-clockwise 180 degree */
    ROT270, /**< rotated anti-clockwise 270 degree */
    FLI000, /**< fliped against y-axis */
    FLI090, /**< rotated 90 degree and then fliped */
    FLI180, /**< rotated 90 degree and then fliped */
    FLI270  /**< rotated 90 degree and then fliped */
  };

  /** Look up tables for rotations and flippings */
  static const Orient _lut_rot090[8];
  static const Orient _lut_rot180[8];
  static const Orient _lut_rot270[8];
  static const Orient _lut_fli000[8];
  static const Orient _lut_fli090[8];
  static const Orient _lut_fli180[8];
  static const Orient _lut_fli270[8];

  /** Default constructor. Create a orientation. */
  inline orient() : _status(ROT000) {}

  /** Create a orientation. */
  inline orient(Orient s) : _status(s) {}

  // Compiler generated copy constructor and operator= are fine

  /** @return a random orientation */
  inline static orient random() {
    return orient(static_cast<Orient>(rand() % 8));
  }

  /** @return the status of the orientation */
  inline Orient status() const { return _status; }

  /**
   * @name Geometry transformations
   */
  //@{

  /** Rotate 90 degree anti-clockwise. */
  inline void rotate090() { _status = _lut_rot090[_status]; }

  /** Rotate 180 degree anti-clockwise. */
  inline void rotate180() { _status = _lut_rot180[_status]; }

  /** Rotate 270 degree anti-clockwise. */
  inline void rotate270() { _status = _lut_rot270[_status]; }

  /** Flip against y-axis */
  inline void flip_y() { _status = _lut_fli000[_status]; }

  /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
  inline void rotate090_then_flip_y() { _status = _lut_fli090[_status]; }

  /** Rotate 180 degree anti-clockwise and then flip against y-axis */
  inline void rotate180_then_flip_y() { _status = _lut_fli180[_status]; }

  /** Rotate 270 degree anti-clockwise and then flip against y-axis */
  inline void rotate270_then_flip_y() { _status = _lut_fli270[_status]; }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate270() { rotate090_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate180() { rotate180_then_flip_y(); }

  /** Flip against y-axis and then rotate 90 degree anti-clockwise. */
  inline void flip_y_then_rotate090() { rotate270_then_flip_y(); }

  /** Flip against x-axis */
  inline void flip_x() { rotate180_then_flip_y(); }

  /** Transform the object obj to this coordinate system */
  template <class geom_obj> inline void xform_to(geom_obj &obj) const;

  /** Transform the object obj from this coordinate system */
  template <class geom_obj> inline void xform_from(geom_obj &obj) const;
  //@}

private:
  Orient _status; /**< orientation status */
};

// Inline member functions

// xxx /** Rotate 90 degree anti-clockwise. */
// xxx inline void orient::rotate090()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = ROT090; break;
// xxx   case ROT090: _status = ROT180; break;
// xxx   case ROT180: _status = ROT270; break;
// xxx   case ROT270: _status = ROT000; break;
// xxx   case FLI000: _status = FLI270; break;
// xxx   case FLI090: _status = FLI000; break;
// xxx   case FLI180: _status = FLI090; break;
// xxx   case FLI270: _status = FLI180; break;
// xxx   }
// xxx }
// xxx
// xxx /** Rotate 180 degree anti-clockwise. */
// xxx inline void orient::rotate180()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = ROT180; break;
// xxx   case ROT090: _status = ROT270; break;
// xxx   case ROT180: _status = ROT000; break;
// xxx   case ROT270: _status = ROT090; break;
// xxx   case FLI000: _status = FLI180; break;
// xxx   case FLI090: _status = FLI270; break;
// xxx   case FLI180: _status = FLI000; break;
// xxx   case FLI270: _status = FLI090; break;
// xxx   }
// xxx }
// xxx
// xxx /** Rotate 270 degree anti-clockwise. */
// xxx inline void orient::rotate270()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = ROT270; break;
// xxx   case ROT090: _status = ROT000; break;
// xxx   case ROT180: _status = ROT090; break;
// xxx   case ROT270: _status = ROT180; break;
// xxx   case FLI000: _status = FLI090; break;
// xxx   case FLI090: _status = FLI180; break;
// xxx   case FLI180: _status = FLI270; break;
// xxx   case FLI270: _status = FLI000; break;
// xxx   }
// xxx }
// xxx
// xxx /** Flip against y-axis */
// xxx inline void orient::flip_y()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = FLI000; break;
// xxx   case ROT090: _status = FLI090; break;
// xxx   case ROT180: _status = FLI180; break;
// xxx   case ROT270: _status = FLI270; break;
// xxx   case FLI000: _status = ROT000; break;
// xxx   case FLI090: _status = ROT090; break;
// xxx   case FLI180: _status = ROT180; break;
// xxx   case FLI270: _status = ROT270; break;
// xxx   }
// xxx }
// xxx
// xxx /** Rotate 90 degree anti-clockwise and then flip against y-axis. */
// xxx inline void orient::rotate090_then_flip_y()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = FLI090; break;
// xxx   case ROT090: _status = FLI180; break;
// xxx   case ROT180: _status = FLI270; break;
// xxx   case ROT270: _status = FLI000; break;
// xxx   case FLI000: _status = ROT270; break;
// xxx   case FLI090: _status = ROT000; break;
// xxx   case FLI180: _status = ROT090; break;
// xxx   case FLI270: _status = ROT180; break;
// xxx   }
// xxx }
// xxx
// xxx /** Rotate 180 degree anti-clockwise and then flip against y-axis */
// xxx inline void orient::rotate180_then_flip_y()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = FLI180; break;
// xxx   case ROT090: _status = FLI270; break;
// xxx   case ROT180: _status = FLI000; break;
// xxx   case ROT270: _status = FLI090; break;
// xxx   case FLI000: _status = ROT180; break;
// xxx   case FLI090: _status = ROT270; break;
// xxx   case FLI180: _status = ROT000; break;
// xxx   case FLI270: _status = ROT090; break;
// xxx   }
// xxx }
// xxx
// xxx /** Rotate 270 degree anti-clockwise and then flip against y-axis */
// xxx inline void orient::rotate270_then_flip_y()
// xxx {
// xxx   switch (_status) {
// xxx   case ROT000: _status = FLI270; break;
// xxx   case ROT090: _status = FLI000; break;
// xxx   case ROT180: _status = FLI090; break;
// xxx   case ROT270: _status = FLI180; break;
// xxx   case FLI000: _status = ROT090; break;
// xxx   case FLI090: _status = ROT180; break;
// xxx   case FLI180: _status = ROT270; break;
// xxx   case FLI270: _status = ROT000; break;
// xxx   }
// xxx }

/** Transform the object obj to this coordinate system */
template <class geom_obj> inline void orient::xform_to(geom_obj &obj) const {
  switch (_status) {
  case ROT000:
    break;
  case ROT090:
    obj.rotate270();
    break;
  case ROT180:
    obj.rotate180();
    break;
  case ROT270:
    obj.rotate090();
    break;
  case FLI000:
    obj.flip_y();
    break;
  case FLI090:
    obj.rotate090_then_flip_y();
    break;
  case FLI180:
    obj.rotate180_then_flip_y();
    break;
  case FLI270:
    obj.rotate270_then_flip_y();
    break;
  }
}

/** Transform the object obj from this coordinate system */
template <class geom_obj> inline void orient::xform_from(geom_obj &obj) const {
  switch (_status) {
  case ROT000:
    break;
  case ROT090:
    obj.rotate090();
    break;
  case ROT180:
    obj.rotate180();
    break;
  case ROT270:
    obj.rotate270();
    break;
  case FLI000:
    obj.flip_y();
    break;
  case FLI090:
    obj.rotate090_then_flip_y();
    break;
  case FLI180:
    obj.rotate180_then_flip_y();
    break;
  case FLI270:
    obj.rotate270_then_flip_y();
    break;
  }
}

// Non-member inline functions

/** Equal. Two orients are equal if and only both their
    statuses are same. */
inline bool operator==(const orient &lhs, const orient &rhs) {
  return lhs.status() == rhs.status();
}

/** Not Equal. @see operator==() */
inline bool operator!=(const orient &lhs, const orient &rhs) {
  return !(lhs == rhs);
}

/** @} */
}

#endif // RECTI_ORIENT_HPP
