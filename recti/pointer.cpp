#include "pointer.hpp"
#include <boost/static_assert.hpp>
#include <interval.hpp>
#include <svgstream.hpp>

using numeric::interval;
using std::ostream;

namespace recti {

template <typename _Tp>
Pointer<_Tp>::Pointer(_Tp x, _Tp y, Pointer<_Tp> *next, Pointer<_Tp> *prev)
    : _x(x), _y(y), _next(next), _prev(prev) {}

template <typename _Tp> void Pointer<_Tp>::next(Pointer<_Tp> *next) {
  _next = next;
}

template <typename _Tp> void Pointer<_Tp>::prev(Pointer<_Tp> *prev) {
  _prev = prev;
}

/** Explicit instantiation */
template class Pointer<int>;
template class Pointer<interval<int>>;
}
