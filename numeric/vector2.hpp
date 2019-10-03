#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iosfwd>
//#include <cmath>  // for sqrt
#include <boost/operators.hpp>
#include <cstdlib> // for rand()

namespace numeric {

/** @addtogroup numeric
 *  @{
 */

/** Generalized 2x1 vector. */
template <typename _Tp> struct vector2 : boost::operators<vector2<_Tp> > {
  _Tp _a1;
  _Tp _a2;

  /** Default Constructor */
  vector2() : _a1(), _a2() {}

  /** Constructor */
  vector2(const _Tp &a) : _a1(a), _a2(a) {}

  /** Constructor */
  vector2(const _Tp &a1, const _Tp &a2) : _a1(a1), _a2(a2) {}

  // Default assignment operator is fine

  /** Construct from another vector */
  template <typename _Up>
  vector2(const vector2<_Up> &v) : _a1(v._a1), _a2(v._a2) {}

  /** Create a random vector */
  inline static vector2<_Tp> random() {
    return vector2<_Tp>(_Tp::random(), _Tp::random());
  }

  /** Add and assign */
  vector2<_Tp> &operator+=(const vector2<_Tp> &v) {
    _a1 += v._a1;
    _a2 += v._a2;
    return *this;
  }

  /** Subtract amd assign */
  template <typename _Up> vector2<_Tp> &operator-=(const vector2<_Up> &v) {
    _a1 -= v._a1;
    _a2 -= v._a2;
    return *this;
  }

  /** Multiply a scalar and assign */
  vector2<_Tp> &operator*=(const _Tp &alpha) {
    _a1 *= alpha;
    _a2 *= alpha;
    return *this;
  }

  /** Divide by a scalar and assign */
  vector2<_Tp> &operator/=(const _Tp &alpha) {
    _a1 /= alpha;
    _a2 /= alpha;
    return *this;
  }

  /** Add (auto-generated) */

  /** Subtract (auto-generated) */

  /** Multiply a scalar */
  // xxx template <typename _Up>
  // xxx vector2<_Tp> operator*(const _Up& alpha) const
  // xxx { return vector2<_Tp>(*this) *= alpha; }

  // Relational operators

  /** Equal */
  bool operator==(const vector2<_Tp> &v) const {
    return _a1 == v._a1 && _a2 == v._a2;
  }

  /** Not equal (auto-generated) */
};

// xxx static_assert(sizeof(vector2<int>)==2*sizeof(int), "Object size
// increased!");

/** Create a random vector */
template <> inline vector2<int> vector2<int>::random() {
  return vector2<int>((rand() - rand()) % 1000, (rand() - rand()) % 1000);
}

// Non-member functions
/** Unary plus */
template <typename _Tp> inline vector2<_Tp> operator+(const vector2<_Tp> &v) {
  return v;
}

/** Unary minus */
template <typename _Tp> inline vector2<_Tp> operator-(const vector2<_Tp> &v) {
  return vector2<_Tp>(-v._a1, -v._a2);
}

/** Multiply by a scalar */
template <typename _Tp>
inline vector2<_Tp> operator*(double alpha, const vector2<_Tp> &v) {
  return vector2<_Tp>(alpha * v._a1, alpha * v._a2);
}

/** Inner product */
template <typename _Tp>
inline _Tp dot(const vector2<_Tp> &a, const vector2<_Tp> &b) {
  return a._a1 * b._a1 + a._a2 * b._a2;
};

// xxx /** Cross product */
// xxx template <typename _Tp>
// xxx inline _Tp cross_product(const vector2<_Tp>& a, const vector2<_Tp>& b)
// xxx { return a._a1 * b._a2 - a._a2 * b._a1; };

/** Determinant */
template <typename _Tp>
inline _Tp det(const vector2<_Tp> &a, const vector2<_Tp> &b) {
  return a._a1 * b._a2 - a._a2 * b._a1;
};

/** 2-norm */
template <typename _Tp> inline double norm2(const vector2<_Tp> &a) {
  return sqrt(dot(a, a));
}

/** 2-norm */
template <typename _Tp> inline double norm(const vector2<_Tp> &a) {
  return norm2(a);
}

/* Print the data */
template <typename _Tp>
std::ostream &operator<<(std::ostream &out, const vector2<_Tp> &v) {
  out << "[" << v._a1 << ", " << v._a2 << "]";
  return out;
}

/** Solve quadratic polynomial equation x^2 - v1*x - v2 = 0 */
template <typename _Tp>
std::ostream &display_roots(std::ostream &out, const vector2<_Tp> &v);

/** @} */
}

#endif
