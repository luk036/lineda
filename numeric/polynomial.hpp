#ifndef NUMERIC_POLYNOMIAL_HPP
#define NUMERIC_POLYNOMIAL_HPP

#include <boost/call_traits.hpp> // for boost::call_traits
#include <boost/config.hpp>      // for BOOST_NO_STDC_NAMESPACE, BOOST_MSVC
#include <cassert>
#include <cmath>
#include <cstdlib> // for std::abs
#include <iostream>
#include <stdexcept> // for std::domain_error
#include <string>    // for std::string implicit constructor
#include <utility>
#include <vector> // polynomial is implemented by vector

inline double norm(double d) { return fabs(d); }

namespace numeric {
/**
 * @addtogroup numeric
 * @{
 */

/** Forward declaration */
template <typename _Tp> class polynomial;
template <typename _Tp> struct vector2;
template <typename _Tp> struct matrix2;

/** Class that will be thrown when divide-by-zero occurs */
class bad_polynomial : public std::domain_error {
public:
  explicit bad_polynomial()
      : std::domain_error("bad polynomial: divide by zero") {}
};

/**
 * Polynomial Class. A polynomial can be defined over any field
 * (real, integer, complex, rational, etc.). For this reason, the
 * polynomial class is templated, which means that it could take as
 * parameters various data types that have the operators {+, *, -, /}
 * defined. The data type with operator + must be an Abelian
 * (commutative) group, the data type with the operator * must be a
 * semi-group (no multiplicative inverses necessary for basic
 * operation), and also + must distribute over *). Polynomial is
 * represented by vector of coefficients. Zero polynomial can
 * be constructed by default constructor.
 */
template <typename _Tp> class polynomial {
  /** @todo Implement polynomial by linked list instead of vector */
  typedef std::vector<_Tp> coeff_vec_type;
  typedef typename boost::call_traits<_Tp>::param_type param_type;
  typedef polynomial<_Tp> _Self;

public:
  /** Default constructor. Create a zero polynomial. */
  inline polynomial() : coeff_vec(1, _Tp()) {}

  /** Construct a constant as a polynomail. */
  inline polynomial(param_type c) : coeff_vec(1, c) {}

  /** Construct a polynomial by a vector of coefficients. */
  inline polynomial(coeff_vec_type v) : coeff_vec(v) { normalize(); }

  /** Construct a polynomial by an array of coefficients. */
  polynomial(const _Tp *a, size_t s) : coeff_vec(a, a + s) { normalize(); }

  /** Construct a polynomial that fits the data x[] and y[].
      Pre-requisite: type _Tp is inversible.
      Pre-condition: all x's distinct.
      Time complexity: O(n^2). Space complexity O(n). */
  polynomial(const _Tp *x, const _Tp *y, size_t s);

  // Computer generated copy constructor and operator=() are fine.

  /** Add assignment from _Tp. */
  inline polynomial<_Tp> &operator=(param_type c) { return assign(c); }

  /** Assign in place. */
  inline polynomial<_Tp> &assign(param_type c);

  /** @return the size of this polynomial. */
  inline size_t size() const { return coeff_vec.size(); }

  /** @return the degree of this polynomial. */
  inline size_t degree() const { return size() - 1; }

  /** @return the ith coefficient of this polynomial. */
  inline const _Tp &get_coeff(size_t i) const { return coeff_vec[i]; }

  // Arithmetic assignment operators

  /** Add and assign */
  template <typename _Up>
  inline polynomial<_Tp> &operator+=(const polynomial<_Up> &p);

  /** Subtract and assign */
  template <typename _Up>
  inline polynomial<_Tp> &operator-=(const polynomial<_Up> &p);

  /** Multiply and assign */
  template <typename _Up>
  inline polynomial<_Tp> &operator*=(const polynomial<_Up> &p);

  /** Divide and assign. Pre-requisite: _Tp is inversible. */
  inline polynomial<_Tp> &operator/=(const polynomial<_Tp> &p);

  /** Remainder. Pre-requisite: _Tp is inversible. */
  template <typename _Up>
  inline polynomial<_Tp> &operator%=(const polynomial<_Up> &p);

  /** Add a constant b and assign */
  template <typename _Up> inline polynomial<_Tp> &operator+=(const _Up &b);

  /** Subtract a constant b and assign */
  template <typename _Up> inline polynomial<_Tp> &operator-=(const _Up &b);

  /** Multiply a constant b and assign */
  template <typename _Up> inline polynomial<_Tp> &operator*=(const _Up &b);

  /** Divide by b and assign. Pre-requisite: _Tp is inversible. */
  template <typename _Up> inline polynomial<_Tp> &operator/=(const _Up &b);

  /** Add */
  inline polynomial<_Tp> operator+(const polynomial<_Tp> &p) const {
    return polynomial<_Tp>(*this) += p;
  }

  /** Subtract */
  inline polynomial<_Tp> operator-(const polynomial<_Tp> &p) const {
    return polynomial<_Tp>(*this) -= p;
  }

  /** Multiply */
  inline polynomial<_Tp> operator*(const polynomial<_Tp> &p) const {
    return polynomial<_Tp>(*this) *= p;
  }

  /** Divide */
  inline polynomial<_Tp> operator/(const polynomial<_Tp> &p) const {
    return polynomial<_Tp>(*this) /= p;
  }

  /** Remainder */
  inline polynomial<_Tp> operator%(const polynomial<_Tp> &p) const {
    return polynomial<_Tp>(*this) %= p;
  }

  /** Add a constant */
  inline polynomial<_Tp> operator+(const _Tp &b) const {
    return polynomial<_Tp>(*this) += b;
  }

  /** Subtract a constant */
  inline polynomial<_Tp> operator-(const _Tp &b) const {
    return polynomial<_Tp>(*this) -= b;
  }

  /** Multiply a constant */
  inline polynomial<_Tp> operator*(const _Tp &b) const {
    return polynomial<_Tp>(*this) *= b;
  }

  /** Divide by a constant */
  inline polynomial<_Tp> operator/(const _Tp &b) const {
    return polynomial<_Tp>(*this) /= b;
  }

  // Comparison operators

  /** @return whether this is a zero polynomial. */
  inline bool operator!() const {
    return coeff_vec.size() == 1 && coeff_vec[0] == _Tp();
  }

  /** Test if this polynomial is asymtotically less than p.
      Polynomial q is asymtotically less than p if and only if
      (1) degree(q) > degree(p) and the coefficient of the highest
      degree of q < 0, or
      (2) degree(q) < degree(p) and the coefficient of the highest
      degree of p > 0, or
      (3) degree(q) == degree(p) and the coefficient of the highest
      degree of q is less than p's.

      Pre-requisite: the coefficent type is less_than_comparable  */
  template <typename _Up> inline bool operator<(const polynomial<_Up> &p) const;

  /** Equal. Two polynomials are equal if their coefficient vector
      have the same degree and all values are equal. */
  template <typename _Up>
  inline bool operator==(const polynomial<_Up> &p) const;

  /** Greater than or equal to. @see operator<(). */
  template <typename _Up>
  inline bool operator>=(const polynomial<_Up> &q) const {
    return !(*this < q);
  }

  /** Less than or equal to. @see operator<(). */
  template <typename _Up>
  inline bool operator<=(const polynomial<_Up> &q) const {
    return !(q < *this);
  }

  /** Greater than. @see operator<(). */
  template <typename _Up>
  inline bool operator>(const polynomial<_Up> &q) const {
    return (q < *this);
  }

  /** Not equal. @see operator<(). */
  template <typename _Up>
  inline bool operator!=(const polynomial<_Up> &q) const {
    return !(*this == q);
  }

  /** @return P(x) the evaluation of this polynomial at x. */
  _Tp operator()(const _Tp &x) const;

  /** @return P(x) the evaluation of this polynomial at x. */
  template <typename _Tp2, typename _Tp3> _Tp3 eval(const _Tp2 &x) const;

  /** @return remainder when polynomial is divided by
      (x^2 - v[1]*x - x[2]) */
  vector2<_Tp> operator()(const vector2<_Tp> &v) const;

  /** @return P'(x) the numerical derivative given x */
  _Tp differentiate(const _Tp &x) const;

  /** @return the numerical derivative given x */
  matrix2<_Tp> differentiate(const vector2<_Tp> &x) const;

  /** @return the first derivative of this polynomial */
  polynomial<_Tp> derivative() const;

  /** @return the anti-derivative (indefinite integral). */
  polynomial<_Tp> anti_derivative() const;

  /** @return the integral at interval [a b] */
  _Tp integrate(const _Tp &a, const _Tp &b) const;

  /** Find a root using Newton's method. Return true if a root is found.
      Pre-requisite: _Tp is in a normed space. */
  inline bool find_root(_Tp &x, int max_it = 100, double tol = 1e-6);

private:
  /** @return P(x) / P'(x) together using Horner rule */
  _Tp horner(const _Tp &x) const;

  /** @return P(v) / P'(v) together using Horner rule */
  vector2<_Tp> horner(const vector2<_Tp> &x) const;

  /** Remove trailing zeros from the coefficient vector. */
  void normalize();

private:
  coeff_vec_type coeff_vec; /**< vector of coefficents */
};

/** Assign in place */
template <typename _Tp>
inline polynomial<_Tp> &polynomial<_Tp>::assign(param_type c) {
  coeff_vec.resize(1);
  coeff_vec[0] = c;
  return *this;
}

/** Unary plus */
template <typename _Tp>
inline polynomial<_Tp> operator+(const polynomial<_Tp> &p) {
  return p;
}

/** Negates the polynomial p */
template <typename _Tp>
inline polynomial<_Tp> operator-(const polynomial<_Tp> &p) {
  return polynomial<_Tp>() -= p;
}

/** Arithmetic assignment operator */
template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator+=(const polynomial<_Up> &p) {
  if (!p)
    return *this;
  size_t n = coeff_vec.size();
  const size_t m = p.size();
  if (n < m) {
    coeff_vec.resize(m, _Tp()); // ???
    n = m;
  }
  for (size_t i = 0; i < m; ++i) {
    coeff_vec[i] += p.get_coeff(i);
  }
  normalize();
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator-=(const polynomial<_Up> &p) {
  if (!p)
    return *this;

  size_t n = coeff_vec.size();
  const size_t m = p.size();
  if (n < m) {
    coeff_vec.resize(m, _Tp()); // ???
    n = m;
  }
  for (size_t i = 0; i < m; ++i) {
    coeff_vec[i] -= p.get_coeff(i);
  }
  normalize();
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator*=(const polynomial<_Up> &p) {
  if (!(*this))
    return *this; // this polynomial is zero
  if (!p)
    return (*this) = _Tp(); // p is zero

  const size_t n = coeff_vec.size();
  const size_t m = p.size();
  coeff_vec_type res(n + m - 1, _Tp());
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      res[i + j] += coeff_vec[i] * p.get_coeff(j);
    }
  }
  coeff_vec = res;
  return *this;
}

template <typename _Tp>
inline polynomial<_Tp> &polynomial<_Tp>::operator/=(const polynomial<_Tp> &p) {
  if (!p)
    throw bad_polynomial();
  if (this == &p)
    return (*this) = _Tp(1);
  if (!(*this))
    return *this;
  const size_t n = size();
  const size_t m = p.size();
  if (n < m)
    return (*this) = _Tp();
  for (size_t i = n - 1; i >= m - 1; --i) {
    coeff_vec[i] /= p.get_coeff(m - 1);
    for (size_t j = 0; j < m - 1; ++j) {
      coeff_vec[j + i - m + 1] -= coeff_vec[i] * p.get_coeff(j);
    }
  }
  std::copy(coeff_vec.begin() + (m - 1), coeff_vec.end(), coeff_vec.begin());
  coeff_vec.resize(n - m + 1);
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator%=(const polynomial<_Up> &p) {
  if (!p)
    throw bad_polynomial();
  if (!(*this))
    return *this;
  if (this == &p)
    return (*this) = _Tp();
  const size_t n = size();
  const size_t m = p.size();
  if (n < m)
    return *this;
  for (size_t i = n - 1; i >= m - 1; --i) {
    _Tp q = coeff_vec[i] / p.get_coeff(m - 1);
    for (size_t j = 0; j < m - 1; ++j) {
      coeff_vec[j + i - m + 1] -= q * p.get_coeff(j);
    }
  }
  coeff_vec.resize(m - 1);
  normalize();
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator+=(const _Up &b) {
  coeff_vec[0] += b;
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator-=(const _Up &b) {
  coeff_vec[0] -= b;
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator*=(const _Up &b) {
  if (_Tp() == b)
    return (*this) = _Tp();

  const size_t n = coeff_vec.size();
  for (size_t i = 0; i < n; ++i) {
    coeff_vec[i] *= b;
  }
  return *this;
}

template <typename _Tp>
template <typename _Up>
inline polynomial<_Tp> &polynomial<_Tp>::operator/=(const _Up &b) {
  if (_Tp() == b)
    throw bad_polynomial();
  const size_t n = coeff_vec.size();
  for (size_t i = 0; i < n; ++i) {
    coeff_vec[i] /= b;
  }
  return *this;
}

// Comparison operators
template <typename _Tp>
template <typename _Up>
inline bool polynomial<_Tp>::operator<(const polynomial<_Up> &p) const {
  const size_t s = size();
  const size_t t = p.size();
  if (s < t)
    return _Tp() < p.get_coeff(t - 1);
  if (s > t)
    return coeff_vec[s - 1] < _Tp();
  return coeff_vec[s - 1] < p.get_coeff(t - 1);
}

template <typename _Tp>
template <typename _Up>
inline bool polynomial<_Tp>::operator==(const polynomial<_Up> &p) const {
  if (size() != p.size())
    return false;
  for (size_t i = 0; i < size(); ++i) {
    if (!(get_coeff(i) == p.get_coeff(i)))
      return false;
  }
  return true;
}

/** @return the evaluation of this polynomial at x. Use Horner rule. */
template <typename _Tp>
inline _Tp polynomial<_Tp>::operator()(const _Tp &x) const {
  const int n = degree();
  auto res = _Tp(0);
  for (int i = n; i >= 0; --i) {
    res *= x; // res = res * x + coeff_vec[i]
    res += coeff_vec[i];
  }
  return res;
}

/** @return the evaluation of this polynomial at x. Use horner rule. */
template <typename _Tp>
template <typename _Tp2, typename _Tp3>
inline _Tp3 polynomial<_Tp>::eval(const _Tp2 &x) const {
  const int n = degree();
  _Tp3 res = _Tp3(0);
  for (int i = n; i >= 0; --i) {
    res *= x; // res = res * x + coeff_vec[i]
    res += coeff_vec[i];
  }
  return res;
}

/** @return the numerical derivative given x */
template <typename _Tp>
inline _Tp polynomial<_Tp>::differentiate(const _Tp &x) const {
  const int n = degree();
  if (n == 0)
    return _Tp();
  _Tp res = coeff_vec[n] * _Tp(n);
  for (int i = n - 1; i > 0; --i) {
    res *= x;
    res += coeff_vec[i] * _Tp(i);
  }
  return res;
}

template <typename _Tp> inline _Tp polynomial<_Tp>::horner(const _Tp &x) const {
  const int n = degree();
  _Tp p0 = coeff_vec[n];
  _Tp p1 = _Tp();
  for (int i = n - 1; i >= 0; --i) {
    p1 *= x; // p1 = p1*x + p0
    p1 += p0;
    p0 *= x; // p0 = p0*x + c[i]
    p0 += coeff_vec[i];
  }
  return p0 / p1;
}

// Normalization
template <typename _Tp> inline void polynomial<_Tp>::normalize() {
  const _Tp zero = _Tp();
  size_t degree = coeff_vec.size() - 1;
  while (degree > 0) {
    if (coeff_vec[degree] != zero)
      break;
    --degree;
  }
  coeff_vec.resize(degree + 1);
}

template <typename _Tp>
inline bool polynomial<_Tp>::find_root(_Tp &x, int max_it, double tol) {
  // Initialization
  for (int iter = 1; iter <= max_it; ++iter) {
    const _Tp d = horner(x);
    if (norm(d) <= tol)
      return true;
    x -= d;
  }
  return false;
}

// Non-member functions

// Add manipulators for output format?
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &os, const polynomial<_Tp> &p) {
  os << "[" << p.get_coeff(0);
  for (size_t i = 1; i < p.size(); ++i) {
    os << ", " << p.get_coeff(i);
  }
  os << "]";
  return os;
}

/** @} */
}

#endif // POLYNOMIAL_HPP
