#ifndef RECTI_POINTER_HPP
#define RECTI_POINTER_HPP

// xxx #ifndef RECTI_POINT_HPP
// xxx #include "point.hpp"
// xxx #endif

namespace recti {

template <typename _Tp> class Pointer;

/** ??? */
template <typename _Tp> class Pointer {
public:
  Pointer(_Tp x, _Tp y, Pointer<_Tp> *next, Pointer<_Tp> *prev);
  _Tp _x, _y;

  // xxx int _No,_num,_Anticlkno;  //_num records the number of the points in
  // the polygon.

  Pointer<_Tp> *_next;
  Pointer<_Tp> *_prev;

  void next(Pointer<_Tp> *next = 0);
  void prev(Pointer<_Tp> *prev = 0);
  _Tp x() const { return _x; }
  _Tp y() const { return _y; }
};
}
#endif // RECTI_POINTER_HPP
