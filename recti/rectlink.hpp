#ifndef RECTI_RECTLINK_HPP
#define RECTI_RECTLINK_HPP

#ifndef RECTI_RECTANGLE_HPP
#include "rectangle.hpp"
#endif

#ifndef RECTI_POINTER_HPP
#include "pointer.hpp"
#endif

#ifndef RECTI_POINT_HPP
#include "point.hpp"
#endif

#ifndef NUMERIC_INTERVAL_HPP
#include "interval.hpp"
#endif
#include <cassert>
#include <iosfwd> // for declaration of std::istream and std::ostream

namespace recti {

template <typename _Tp> class Rectlink;

/** ??? */
template <class _Tp> class Rectlink : public Rectangle<_Tp> {
  typedef Rectangle<_Tp> _Base;

public:
  Rectlink(const Pointer<_Tp> &p, const _Tp &width, const _Tp &height);
};
}
#endif
