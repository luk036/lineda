#ifndef KT_RECTANGLE_HPP
#define KT_RECTANGLE_HPP

#include <point.hpp>

using namespace recti;
using namespace std;

const int INTMAX = 32767;
const signed int INTMIN = -32768;

const int INFINITE = INTMAX;     // The maximal number
const signed int START = INTMIN; // The minimal number
//####namespace cgl {

/** kd tree rectangle */
class kt_rectangle {
public:
  point _top_left, _bottom_right;
  // xxx  static int INFINITE;// = 32767;//INTMAX;
  // xxx  static signed int START ;//= -32768;//INTMIN;

  /** Constructor */
  kt_rectangle() {
    _top_left = point(START, START);           // ORIGIN;
    _bottom_right = point(INFINITE, INFINITE); // MAX_INT
  }

  kt_rectangle(const int &x1, const int &y1, const int &x2, const int &y2)
      : _top_left(x1, y1), _bottom_right(x2, y2) {}

  kt_rectangle(int &x1, int &y1, int &x2, int &y2)
      : _top_left(x1, y1), _bottom_right(x2, y2) {}

  kt_rectangle(const point &top_left_val, const point &bottom_right_val)
      : _top_left(top_left_val), _bottom_right(bottom_right_val) {}

  kt_rectangle(point &top_left_val, point &bottom_right_val)
      : _top_left(top_left_val), _bottom_right(bottom_right_val) {}

  kt_rectangle(const kt_rectangle &rect) = default;

  kt_rectangle operator=(const kt_rectangle rect) {
    kt_rectangle rect1;
    rect1._top_left = rect._top_left;
    rect1._bottom_right = rect._bottom_right;
    return rect1;
  }

  // sss  void set_frame()
  // sss  {
  // sss    START = INT_MIN;
  // sss    INFINITE = INT_MAX;
  // sss  }
  // sss
  // sss  void set_frame(int& i1, int& i2)
  // sss  {
  // sss    START = i1;
  // sss    INFINITE = i2;
  // sss  }

  static kt_rectangle random() {
    return kt_rectangle(point::random(), point::random());
  }

  int tlx() // top_left x
  {
    return _top_left.x();
  }

  int tly() { return _top_left.y(); }

  int brx() // bottom_right x
  {
    return _bottom_right.x();
  }

  int bry() { return _bottom_right.y(); }

  bool operator==(kt_rectangle &r) {
    return (_top_left == r._top_left && _bottom_right == r._bottom_right);
  }

  bool is_valid() {
    int x1 = _top_left.x();
    int y1 = _top_left.y();
    int x2 = _bottom_right.x();
    int y2 = _bottom_right.y();
    if (x1 <= x2 && y1 <= y2) {
      return true;
    }
    // Get the valid rectangular range.
    if (y1 > y2) {
      _top_left = point(x1, y2);
      _bottom_right = point(x2, y1);
      y1 = _top_left.y();
      y2 = _bottom_right.y();
    }
    if (x1 > x2) {
      _top_left = point(x2, y1);
      _bottom_right = point(x1, y2);
    }
    return true;
  }

  //  rectangle operator = (const point& top_left, const point& bottom_right)
  //{
  // kt_rectangle rect(top_left, bottom_right);
  // return rect;
  // }

  void print_rect();

  bool is_fully_contained(const kt_rectangle &rect) const {
    if (_top_left.x() >= rect._top_left.x() &&
        _top_left.y() >= rect._top_left.y() &&
        _bottom_right.x() <= rect._bottom_right.x() &&
        _bottom_right.y() <= rect._bottom_right.y()) {
      return true;
    }
    return false;
  }

  bool is_overlap(const kt_rectangle &rect) const {
    if (_bottom_right.x() < rect._top_left.x()) {
      return false;
    }
    if (_bottom_right.y() < rect._top_left.y()) {
      return false;
    }
    if (_top_left.x() > rect._bottom_right.x()) {
      return false;
    }
    if (_top_left.y() > rect._bottom_right.y()) {
      return false;
    }
    return true;
  }

  bool intersect(kt_rectangle &rect) const // unuseful?
  {
    kt_rectangle r = *this;
    if (this->is_overlap(rect) && !(this->is_fully_contained(rect)) &&
        !(rect.is_fully_contained(r))) {
      return true;
    }
    return false;
  }
};

//####} //cgl
#endif
