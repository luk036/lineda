#ifndef NUMERIC_MATRIX2_HPP
#define NUMERIC_MATRIX2_HPP

//#include <cstring>
#include <boost/operators.hpp>
#include <cstdlib> // for rand()
#include <iosfwd>

namespace numeric {
/**
 * @addtogroup numeric
 * @{
 */

// Forward declaration
template <typename _Tp> struct vector2;

/**
 * Generalized 2x2 matrix.
 *
 * <pre>
 *      [      |   ]
 *      [  a11 |a12]
 *      [      |   ]
 *      [ -----+---]
 *      [  a21 |a22]
 * </pre>
 */
template <typename _Tp>
struct matrix2 : boost::operators<matrix2<_Tp> >,
                 boost::operators2<matrix2<_Tp>, _Tp> {
  _Tp _a11, _a12;
  _Tp _a21, _a22;

  /** Default Constructor */
  matrix2() : _a11(_Tp()), _a12(_Tp()), _a21(_Tp()), _a22(_Tp()) {}

  /** Constructor */
  inline matrix2(const _Tp &a) : _a11(a), _a12(_Tp()), _a21(_Tp()), _a22(a) {}

  /** Constructor */
  inline matrix2(const _Tp &a11, const _Tp &a12, const _Tp &a21, const _Tp &a22)
      : _a11(a11), _a12(a12), _a21(a21), _a22(a22) {}

  /** Construct a matrix from another matrix */
  template <typename _Up>
  inline matrix2(const matrix2<_Up> &m)
      : _a11(m._a11), _a12(m._a12), _a21(m._a21), _a22(m._a22) {}

  // Default constructor and assignment operator are fine

  /** Create a random matrix */
  inline static matrix2<_Tp> random() {
    return matrix2<_Tp>(_Tp::random(), _Tp::random(), _Tp::random(),
                        _Tp::random());
  }

  /** Add and assign */
  inline matrix2<_Tp> &operator+=(const matrix2<_Tp> &m) {
    _a11 += m._a11;
    _a12 += m._a12;
    _a21 += m._a21;
    _a22 += m._a22;
    return *this;
  }

  /** Substract and assign */
  inline matrix2<_Tp> &operator-=(const matrix2<_Tp> &m) {
    _a11 -= m._a11;
    _a12 -= m._a12;
    _a21 -= m._a21;
    _a22 -= m._a22;
    return *this;
  }

  /** Multiply and assign */
  inline matrix2<_Tp> &operator*=(const matrix2<_Tp> &m) {
    return *this = matrix2<_Tp>(
               _a11 * m._a11 + _a12 * m._a21, _a11 * m._a12 + _a12 * m._a22,
               _a21 * m._a11 + _a22 * m._a21, _a21 * m._a12 + _a22 * m._a22);
  }

  /** Multiply a scalar and assign */
  inline matrix2<_Tp> &operator*=(const _Tp &alpha) {
    _a11 *= alpha;
    _a12 *= alpha;
    _a21 *= alpha;
    _a22 *= alpha;
    return *this;
  }

  // xxx /** Add */
  // xxx inline matrix2<_Tp> operator+(const matrix2<_Tp>& m) const
  // xxx {
  // xxx   return matrix2<_Tp>(*this) += m;
  // xxx };
  // xxx
  // xxx /** Subtract */
  // xxx inline matrix2<_Tp> operator-(const matrix2<_Tp>& m) const
  // xxx {
  // xxx   return matrix2<_Tp>(*this) -= m;
  // xxx };
  // xxx
  // xxx /** Multiply */
  // xxx inline matrix2<_Tp> operator*(const matrix2<_Tp>& m) const
  // xxx {
  // xxx   return matrix2<_Tp>(_a11 * m._a11 + _a12 * m._a21,
  // xxx 			  _a11 * m._a12 + _a12 * m._a22,
  // xxx 			  _a21 * m._a11 + _a22 * m._a21,
  // xxx 			  _a21 * m._a12 + _a22 * m._a22);
  // xxx };
  // xxx
  // xxx /** Matrix-vector product */
  // xxx inline vector2<_Tp> operator*(const vector2<_Tp>& v) const
  // xxx {
  // xxx   return vector2<_Tp>(_a11 * v._a1 + _a12 * v._a2,
  // xxx 			  _a21 * v._a1 + _a22 * v._a2);
  // xxx };
  // xxx
  // xxx /** Multiply a scalar */
  // xxx template <typename _Up>
  // xxx inline matrix2<_Tp> operator*(const _Up& alpha) const
  // xxx { return matrix2<_Tp>(*this) *= alpha; }

  // Relational operators
  /** Equal */
  inline bool operator==(const matrix2<_Tp> &m) const {
    return _a11 == m._a11 && _a12 == m._a12 && _a21 == m._a21 && _a22 == m._a22;
  }

  // xxx /** Not equal */
  // xxx template <typename _Up>
  // xxx inline bool operator!=(const matrix2<_Up>& m) const
  // xxx { return !((*this) == m); }

  /** @return the transpose */
  inline matrix2<_Tp> transpose() const {
    return matrix2<_Tp>(_a11, _a21, _a12, _a22);
  }

  /** @return whether m is symmetric */
  inline bool is_symmetric() const { return _a12 == _a21; }

  // xxx // Just indicate that two operations can be executed in parallel
  // xxx template <class _Up1, class _Up2, class _Up3, class _Up4>
  // xxx inline void parallel(const _Up1& , const _Up2& ,
  // xxx                      const _Up3& , const _Up4& ) const {}
};

// xxx static_assert(sizeof(matrix2<double>)==4*sizeof(double), "Object size
// increased!");

/** Create a random matrix */
template <> inline matrix2<int> matrix2<int>::random() {
  return matrix2<int>((rand() - rand()) % 1000, (rand() - rand()) % 1000,
                      (rand() - rand()) % 1000, (rand() - rand()) % 1000);
}

// Non-member functions
// xxx /** Unary plus */
// xxx template <typename _Tp>
// xxx inline matrix2<_Tp> operator+(const matrix2<_Tp>& m) { return m; }

/** Unary minus */
template <typename _Tp> inline matrix2<_Tp> operator-(const matrix2<_Tp> &m) {
  return matrix2<_Tp>(-m._a11, -m._a12, -m._a21, -m._a22);
}

// xxx /** Multiply a scalar */
// xxx template <typename _Tp>
// xxx inline matrix2<_Tp> operator*(double alpha, const matrix2<_Tp>& m)
// xxx {
// xxx   return matrix2<_Tp>(alpha * m._a11, alpha * m._a12,
// xxx 			alpha * m._a21, alpha * m._a22);
// xxx }

/* Calculate inv(A)*w  */
template <typename _Tp>
vector2<_Tp> operator/(const vector2<_Tp> &w, const matrix2<_Tp> &m);

/* Print the data */
template <typename _Tp>
std::ostream &operator<<(std::ostream &out, const matrix2<_Tp> &m) {
  out << "[" << m._a11 << ", " << m._a12 << "; " << m._a21 << ", " << m._a22
      << "]";
  return out;
}

// Specializations

// xxx /** @return the transpose of m */
// xxx template <>
// xxx inline matrix2<int> matrix2<int>::transpose() const
// xxx {
// xxx   return matrix2<int>(_a11, _a21,
// xxx 			_a12, _a22);
// xxx }
// xxx
// xxx /** @return whether m is symmetric */
// xxx template <>
// xxx inline bool matrix2<int>::is_symmetric() const
// xxx { return _a12 == _a21; }
// xxx
// xxx
// xxx /** @return the transpose of m */
// xxx template <>
// xxx inline matrix2<double> matrix2<double>::transpose() const
// xxx {
// xxx   return matrix2<double>(_a11, _a21,
// xxx 			   _a12, _a22);
// xxx }
// xxx
// xxx /** @return whether m is symmetric */
// xxx template <>
// xxx inline bool matrix2<double>::is_symmetric() const
// xxx { return _a12 == _a21; }

/** @} */
}
#endif
