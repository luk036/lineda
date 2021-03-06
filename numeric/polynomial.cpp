#include "polynomial.hpp"
#include "matrix2.hpp"
#include "vector2.hpp"
#include <aa.h>
#include <complex>

using std::complex;
using namespace boost;
using namespace std::rel_ops;

namespace numeric {

/** Construct a polynomial that fits the data x[] and y[] by using the
    Newton's interpolation formula (c.f. the book "Introduction to
    Numerical Analysis, J. Stoer and R. Bulirsch, p.43)
    Pre-requisites: type _Tp is inversible.
    Pre-condition: all x's distinct. */
template <typename _Tp>
polynomial<_Tp>::polynomial(const _Tp *x, const _Tp *y, size_t s)
    : coeff_vec() {
  assert(s > 0);
  int i, j;
  std::vector<_Tp> a(s), t(s);

  const int n = s - 1;

  // Calculate the divided differences
  for (i = 0; i <= n; ++i) {
    t[i] = y[i];
    for (j = i - 1; j >= 0; --j) {
      assert(x[i] != x[j]); // assumed all x's distinct
      t[j] = (t[j + 1] - t[j]) / (x[i] - x[j]);
    }
    a[i] = t[0];
  }

  // Construct the polynomial
  assign(a[n]);
  _Tp v[2];
  v[1] = 1.0;
  for (i = n - 1; i >= 0; --i) {
    v[0] = -x[i];
    (*this) *= polynomial<_Tp>(v, 2);
    (*this) += a[i];
  }

  // normalize();
}

/** @return the remainder linear polynomial A*x + B when divided by
    x^2 - v(1)*x - v(2). Use special horner rule.
    Pre-condition: degree of polynomial >= 2
    Reference:
    J. Stoer and R. Bulirsch, Introduction to Numerical Analysis,
    Springer-Verlag, 2nd ed, 1993. */
template <>
vector2<double> polynomial<double>::operator()(const vector2<double> &v) const {
  const int n = degree();
  assert(n >= 2);
  const double r = v._a1;
  const double q = v._a2;
  double b1 = 0.0;
  double b0 = coeff_vec[n];
  double A = b0 * r + coeff_vec[n - 1];
  for (int i = n - 2; i > 0; --i) {
    b1 = A;
    A = b0 * q + b1 * r + coeff_vec[i];
    b0 = b1;
  }
  const double B = b1 * q + coeff_vec[0];
  return vector2<double>(A, B);
}

/** @return the remainder matrix when the derivative of
    polynomail is divided by x^2 - v(1)*x - v(2). Use special horner rule.
    Pre-condition: degree of polynomial >= 2
    Reference:
    J. Stoer and R. Bulirsch, Introduction to Numerical Analysis,
    Springer-Verlag, 2nd ed, 1993. */
template <>
matrix2<double>
polynomial<double>::differentiate(const vector2<double> &v) const {
  const int n = degree();
  assert(n >= 3);
  const double r = v._a1;
  const double q = v._a2;
  double b1 = 0.0;
  double b0 = coeff_vec[n];
  double A = b0 * r + coeff_vec[n - 1];
  double A1 = 0.0;
  double c0 = 0.0;
  double c1 = 0.0;
  for (int i = n - 2; i > 0; --i) {
    b1 = A;
    c1 = A1;
    A1 = c0 * q + c1 * r + b0;
    c0 = c1;
    A = b0 * q + b1 * r + coeff_vec[i];
    b0 = b1;
  }
  const double B1 = c1 * q + b1;
  // const double B = b1*q + coeff_vec[1];
  return matrix2<double>(A1 * r + B1, A1, A1 * q, B1);
}

/** @return together the remainder linear polynomial A*x + B when divided by
    x^2 - v(1)*x - v(2) and the remainder matrix when the derivative of
    polynomail is divided by x^2 - v(1)*x - v(2). Use special horner rule.
    Pre-condition: degree of polynomial >= 3
    Reference:
    J. Stoer and R. Bulirsch, Introduction to Numerical Analysis,
    Springer-Verlag, 2nd ed, 1993. */
template <>
vector2<double> polynomial<double>::horner(const vector2<double> &v) const {
  const int n = degree();
  assert(n >= 3);
  const double r = v._a1;
  const double q = v._a2;
  double b1 = 0.0;
  double b0 = coeff_vec[n];
  double A = b0 * r + coeff_vec[n - 1];
  double A1 = 0.0;
  double c0 = 0.0;
  double c1 = 0.0;
  for (int i = n - 2; i > 0; --i) {
    b1 = A;
    c1 = A1;
    A1 = c0 * q + c1 * r + b0;
    c0 = c1;
    A = b0 * q + b1 * r + coeff_vec[i];
    b0 = b1;
  }
  const double B1 = c1 * q + b1;
  const double B = b1 * q + coeff_vec[1];
  return vector2<double>(A, B) / matrix2<double>(A1 * r + B1, A1, A1 * q, B1);
}

/** Explicit instantiation */
template class polynomial<double>;
template class polynomial<complex<double>>;
// xxx template class polynomial<polynomial<double> >;

template polynomial<float>::polynomial(const float *x, const float *y,
                                       size_t s);

template polynomial<complex<float>>::polynomial(const complex<float> *x,
                                                const complex<float> *y,
                                                size_t s);

template polynomial<AAF>::polynomial(const AAF *x, const AAF *y, size_t s);
}
