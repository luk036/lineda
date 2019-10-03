#include "pointer.hpp"
#include <iostream>
//#include <svgstream.hpp>
//#include <boost/static_assert.hpp>
//#include <interval.hpp>

// using numeric::interval;
// using std::ostream;
using namespace std;

/** @constructor with x, y, next, prev
 *  isRefered is made false meaning the pointer<_Tp>
 *  not refered yet
 */
template <typename _Tp>
Pointer<_Tp>::Pointer(_Tp x, _Tp y, Pointer<_Tp> *next, Pointer<_Tp> *prev)
    : _x(x), _y(y), _next(next), _prev(prev), _isRefered(false) {}

/** @constructor with a node p
 *  only the coordinate of p is copied
 *  isRefered is made false meaning the pointer<_Tp>
 *  not refered yet
 */
template <typename _Tp> Pointer<_Tp>::Pointer(Pointer<_Tp> *p) {
  _x = p->_x;
  _y = p->_y;
  _isRefered = false;
}

/** @set _next as next */
template <typename _Tp> void Pointer<_Tp>::next(Pointer<_Tp> *next) {
  _next = next;
}

/** @set _prev as prev */
template <typename _Tp> void Pointer<_Tp>::prev(Pointer<_Tp> *prev) {
  _prev = prev;
}

/** @return whether the two nodes have equal coordinate */
template <typename _Tp> bool Pointer<_Tp>::equal(Pointer<_Tp> *node) {
  return _x == node->_x && _y == node->_y;
}

/** @return whether the line of the node and next node
 *  is vertical
 *  assert the situation of repeated nodes will not occur here
 */
template <typename _Tp> bool Pointer<_Tp>::nextvertical() {
  return _x == _next->_x && _y != _next->_y;
}

/** @return whether the line of the node and next node
 *  is horizontal
 *  assert the situation of repeated nodes will not occur here
 */
template <typename _Tp> bool Pointer<_Tp>::nexthorizontal() {
  return _x != _next->_x && _y == _next->_y;
}

/** @return whether the line of the node and prevenient
 *  node is vertical
 *  assert the situation of repeated nodes will not occur here
 */
template <typename _Tp>
bool Pointer<_Tp>::prevvertical() { // cout << "_x: " << _x << "_prev->_x: " <<
                                    // _prev->_x
  //     << "_y: " << _y << "_prev->_y: " << _prev->_y << endl;
  return _x == _prev->_x && _y != _prev->_y;
}

/** @return whether the line of the node and prevenient
 *  node is horizontal
 *  assert the situation of repeated nodes will not occur here
 */
template <typename _Tp> bool Pointer<_Tp>::prevhorizontal() {
  return _x != _prev->_x && _y == _prev->_y;
}

template <typename _Tp> bool Pointer<_Tp>::nextdiagonal() {
  return _x != _next->_x && _y != _next->_y;
}

template <typename _Tp> bool Pointer<_Tp>::prevdiagonal() {
  return _x != _prev->_x && _y != _prev->_y;
}

/** @print the coordinate of the current point */
template <typename _Tp> void Pointer<_Tp>::print() {
  cout << "(" << _x << "," << _y << ")" << endl;
}

/** Explicit instantiation */
template class Pointer<int>;
// template class Pointer<interval<int> >;
