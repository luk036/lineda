#include "point_set.hpp"
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <interval.hpp>

using numeric::interval;
using namespace boost::lambda;

namespace recti {
/** Translate this Point_set by (delta_x, delta_y). */
template <typename _Tp>
void Point_set<_Tp>::translate(int delta_x, int delta_y) {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                _1 += Point<int>(delta_x, delta_y));
}

/** Rotate 90 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate090() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate090));
}

/** Rotate 180 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate180() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate180));
}

/** Rotate 270 degree anti-clockwise.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate270() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate270));
}

/** Flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::flip_y() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::flip_y));
}

/** Rotate 90 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate090_then_flip_y() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate090_then_flip_y));
}

/** Rotate 180 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate180_then_flip_y() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate180_then_flip_y));
}

/** Rotate 270 degree anti-clockwise and then flip against y-axis.
    Post-condition: _ll is left below _ur */
template <typename _Tp> void Point_set<_Tp>::rotate270_then_flip_y() {
  std::for_each(_pt_vec.begin(), _pt_vec.end(),
                std::mem_fun_ref(&Point<_Tp>::rotate270_then_flip_y));
}

/** Minkowski sum with a point q and assign */
template <typename _Tp>
template <typename _Up>
Point_set<_Tp> &Point_set<_Tp>::operator+=(const Point<_Up> &q) {
  std::for_each(_pt_vec.begin(), _pt_vec.end(), _1 += var(q));
  return *this;
}

/** Minkowski difference with a point q and assign */
template <typename _Tp>
template <typename _Up>
Point_set<_Tp> &Point_set<_Tp>::operator-=(const Point<_Up> &q) {
  std::for_each(_pt_vec.begin(), _pt_vec.end(), _1 -= var(q));
  return *this;
}

/** Explicit instantiation */
template class Point_set<int>;
template class Point_set<interval<int>>;
}
