#ifndef _POSYNOMIAL_HPP
#define _POSYNOMIAL_HPP

#include "monomial.hpp"
#include <boost/operators.hpp>
#include <cassert>
#include <valarray>
#include <vector>

/**
 * Reference:
 *  S.P. Boyd, S.J. Kim and L.Vandenberghe and A. Hassibi.
 *  A Tutorial on Geometric Programming. Available at
 *  http://www.standford.edu/~boyd/gp_tutorial.html
 */

/**
 * Posynomial function. A sum of one or more monomials, i.e., a
 * function of the form
 *
 *  f(x) = \sum_{k=1}^{K} c_k x_1^{a_{1k}} x_2^{a_{2k}} ... x_n^{a_{nk}},
 *
 * where $c_k$ > 0, is called a posynomial function or, more simply, a
 * posynomial (with $K$ terms, in the variables $x_1$, ..., $x_n$. The
 * term `posynomial' is meant to suggest a combination of `positive'
 * and `polynomial'.
 *
 * Any monomial is also a posynomial. Posynomials are close under
 * addition, multiplication, and positive scaling. Posynomials can be
 * divided by monomials (with the result also a posynomial): If $f$ is
 * a posynomial and $g$ is a monomial, then $f/g$ is a posynomial.
 * Note that <_Tp> could be <double> or <AAF>
 */
template <typename _Tp>
class posynomial : boost::operators<posynomial<_Tp> >,
                   boost::operators2<posynomial<_Tp>, _Tp>,
                   boost::operators2<posynomial<_Tp>, monomial<_Tp> > {
  typedef std::valarray<double> Vec;
  typedef posynomial<_Tp> _Self;

public:
  /** Constructor */
  explicit posynomial(size_t n, size_t N) : _M(N, monomial<_Tp>(n)), _p(N) {
    assert(_M.size() == _p.size());
  }

  /** Constructor */
  posynomial(const monomial<_Tp> &m) : _M(1, m), _p(1) {
    assert(_M.size() == _p.size());
  }

  /** Constructor (for AAF -> double) */
  template <typename Up>
  posynomial(const posynomial<Up> &p, const Up &polarity);

  /** Destructor */
  ~posynomial() {}

  /** Add and assign */
  _Self &operator+=(const monomial<_Tp> &m) {
    _M.push_back(m);
    _p.resize(_p.size() + 1);
    assert(_M.size() == _p.size());
    return *this;
  }

  /** Add and assign */
  _Self &operator+=(const _Self &P) {
    for (size_t i = 0; i < P._M.size(); ++i)
      _M.push_back(P._M[i]);
    _p.resize(_p.size() + P._M.size());
    assert(_M.size() == _p.size());
    return *this;
  }

  /** Multiply and assign */
  _Self &operator*=(const monomial<_Tp> &m) {
    for (size_t i = 0; i < _M.size(); ++i)
      _M[i] *= m;
    assert(_M.size() == _p.size());
    return *this;
  }

  /** Divide and assign */
  _Self &operator/=(const monomial<_Tp> &m) {
    for (size_t i = 0; i < _M.size(); ++i)
      _M[i] /= m;
    assert(_M.size() == _p.size());
    return *this;
  }

  /** Multiply and assign */
  _Self &operator*=(const _Tp &c) {
    for (size_t i = 0; i < _M.size(); ++i)
      _M[i] *= c;
    assert(_M.size() == _p.size());
    return *this;
  }

  /** Divide and assign */
  _Self &operator/=(const _Tp &c) {
    for (size_t i = 0; i < _M.size(); ++i)
      _M[i] /= c;
    assert(_M.size() == _p.size());
    return *this;
  }

  // xxx /** Add */
  // xxx _Self operator+(const monomial<_Tp>& m) const
  // xxx { return _Self(*this) += m; }
  // xxx
  // xxx /** Add */
  // xxx _Self operator+(const _Self& P) const
  // xxx { return _Self(*this) += P; }
  // xxx
  // xxx /** Multiply */
  // xxx _Self operator*(const monomial<_Tp>& m) const
  // xxx { return _Self(*this) *= m; }
  // xxx
  // xxx /** Divide */
  // xxx _Self operator/(const monomial<_Tp>& m) const
  // xxx { return _Self(*this) /= m; }
  // xxx
  // xxx /** Multiply */
  // xxx _Self operator*(const _Tp& c) { return _Self(*this) *= c; }
  // xxx
  // xxx /** Divide */
  // xxx _Self operator/(const _Tp& c) { return _Self(*this) /= c; }

  /** Multiply. @todo simplify the result. */
  _Self &operator*=(const _Self &P) {
    _Self res(_M[0]._a.size(), _M.size() * P._M.size());
    size_t k = 0;
    for (size_t i = 0; i < _M.size(); ++i) {
      for (size_t j = 0; j < P._M.size(); ++j) {
        res._M[k++] = _M[i] * P._M[j];
      }
    }
    assert(_M.size() == _p.size());
    *this = res;
    return *this;
  }

  /** Function evaluation of log(f(exp(y))). */
  template <typename _Up> _Tp operator()(const std::valarray<_Up> &y) const {
    assert(_M[0]._a.size() == y.size());
    const size_t N = _M.size();
    for (size_t i = 0; i < N; ++i)
      _p[i] = _M[i](y);

    if (_M.size() == 1) // monomial
      return _p[0];

    // f <- log(sum_i(exp(y_i)))
    for (size_t i = 0; i < N; ++i) {
      _p[i] = exp(_p[i]);
    }

    const _Tp sum = _p.sum();
    return log(sum);
  }

  /** Gradient of log(f(exp(y))). Precondition: call f(y) previously */
  template <typename _Up>
  std::valarray<_Tp> gradient(const std::valarray<_Up> &y) const {
    assert(_M[0]._a.size() == y.size());
    const size_t n = y.size();
    const size_t N = _M.size();
    std::valarray<_Tp> g(n);

    if (N == 1) { // monomial
      const Vec &gt = _M[0].gradient(y);
      for (size_t i = 0; i < n; ++i) {
        g[i] = gt[i];
      }
      return g;
    }

    // g = Aj' * (exp(yj)./sum(exp(yj)));
    // Note that exp(yj) has been previous calculated in _p during the
    // function evaluation.
    std::valarray<_Tp> z(N);
    for (size_t i = 0; i < N; ++i) {
      z[i] = _p[i];
    }

    _Tp sum = z.sum();
    z /= sum;

    for (size_t i = 0; i < n; ++i) {
      g[i] = 0.;
      for (size_t l = 0; l < N; ++l) {
        g[i] += _M[l]._a[i] * z[l];
      }
    }

    return g;
  }

public:
  std::vector<monomial<_Tp> > _M; /**< vector of monomials */
  mutable std::valarray<_Tp> _p; /**< keep track intermediate result */
  /* _p is important when affine arithmetic is used */

public:
  posynomial(const _Self &Q) : _M(Q._M), _p(Q._p) {}

public:
  _Self &operator=(const _Self &Q) {
    _M = Q._M;

    // Note that _p = Q._p will not work.
    // (a bug in g++'s std::valarray<>::operator=())
    // Use the workaround below.
    _p.resize(Q._p.size()); // explicitly resize p
    for (size_t i = 0; i < _p.size(); ++i)
      _p[i] = Q._p[i]; // assign componentwise

    assert(_M.size() == _p.size());
    return *this;
  }
};

#endif
