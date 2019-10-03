#ifndef RANDVAR_HPP
#define RANDVAR_HPP

#include <boost/call_traits.hpp> // for boost::call_traits
#include <boost/lambda/lambda.hpp>
#include <cmath>
#include "event.hpp"
#include <vector> // randvar is implemented by vector

using namespace boost::lambda;

namespace lineda {
/**
 * @addtogroup lineda
 * @{
 */

/** Forward declaration */
template <typename _Tp> class randvar;

/**
 * Random variable
 */
template <typename _Tp> class randvar {
  typedef randvar<_Tp> _Self;
  typedef typename boost::call_traits<_Tp>::param_type param_type;

public:
  /** Default constructor. Create a zero randvar. */
  inline randvar() : _values(1, _Tp()), _probabilities(1, 0.) {}

  /** Construct a constant as a polynomail. */
  inline randvar(param_type c) : _values(1, c), _probabilities(1, 1.) {}

  /** Construct a randvar by a vector of coefficients. */
  inline randvar(const _Tp v[], const double p[], size_t N)
      : _values(v, v + N), _probabilities(p, p + N) {}

  // Computer generated copy constructor and operator=() are fine.

  // Arithmetic assignment operators
  /** Add a constant b and assign */
  inline randvar<_Tp> &operator+=(const _Tp &b) {
    std::for_each(_values.begin(), _values.end(), _1 += b);
    return *this;
  }

  /** Subtract a constant b and assign */
  inline randvar<_Tp> &operator-=(const _Tp &b) {
    std::for_each(_values.begin(), _values.end(), _1 -= b);
    return *this;
  }

  /** Multiply a constant b and assign */
  inline randvar<_Tp> &operator*=(const _Tp &b) {
    std::for_each(_values.begin(), _values.end(), _1 *= b);
    return *this;
  }

  /** Divide a constant b and assign */
  inline randvar<_Tp> &operator/=(const _Tp &b) {
    std::for_each(_values.begin(), _values.end(), _1 /= b);
    return *this;
  }

  // xxx /** Add a constant */
  // xxx inline randvar<_Tp> operator+ (const _Tp& b) const
  // xxx { return randvar<_Tp>(*this) += b; }
  // xxx
  // xxx /** Subtract a constant */
  // xxx inline randvar<_Tp> operator- (const _Tp& b) const
  // xxx { return randvar<_Tp>(*this) -= b; }
  // xxx
  // xxx /** Multiply a constant */
  // xxx inline randvar<_Tp> operator* (const _Tp& b) const
  // xxx { return randvar<_Tp>(*this) *= b; }
  // xxx
  // xxx /** Divide by a constant */
  // xxx inline randvar<_Tp> operator/ (const _Tp& b) const
  // xxx { return randvar<_Tp>(*this) /= b; }

  /** Add and assign */
  inline randvar<_Tp> &operator+=(const randvar<_Tp> &p);

  /** Subtract and assign */
  inline randvar<_Tp> &operator-=(const randvar<_Tp> &p);

  /** Multiply and assign */
  inline randvar<_Tp> &operator*=(const randvar<_Tp> &p);

  /** Divide and assign. Pre-requisite: _Tp is inversible. */
  inline randvar<_Tp> &operator/=(const randvar<_Tp> &p);

  /** Remainder. Pre-requisite: _Tp is inversible. */
  inline randvar<_Tp> &operator%=(const randvar<_Tp> &p);

  // xxx /** Add */
  // xxx inline randvar<_Tp> operator+ (const randvar<_Tp>& p) const
  // xxx { return randvar<_Tp>(*this) += p; }
  // xxx
  // xxx /** Subtract */
  // xxx inline randvar<_Tp> operator- (const randvar<_Tp>& p) const
  // xxx { return randvar<_Tp>(*this) -= p; }
  // xxx
  // xxx /** Multiply */
  // xxx inline randvar<_Tp> operator* (const randvar<_Tp>& p) const
  // xxx { return randvar<_Tp>(*this) *= p; }
  // xxx
  // xxx /** Divide */
  // xxx inline randvar<_Tp> operator/ (const randvar<_Tp>& p) const
  // xxx { return randvar<_Tp>(*this) /= p; }
  // xxx
  // xxx /** Remainder */
  // xxx inline randvar<_Tp> operator% (const randvar<_Tp>& p) const
  // xxx { return randvar<_Tp>(*this) %= p; }

  // Comparison operators
  template <typename _Up>
  inline event<_Tp> operator<(const randvar<_Up> &p) const;

  template <typename _Up>
  inline event<_Tp> operator==(const randvar<_Up> &p) const;

private:
  std::vector<_Tp> _values;
  std::vector<double> _probabilities;
};

template <typename _Tp>
inline event<_Tp> operator<(const randvar<_Tp> &p, const randvar<_Tp> &q) {
  size_t i, j;
  double probability = 0.;
  for (i = 0; i < p._values.size(); ++i) {
    for (j = 0; j < q._values.size(); ++j) {
      if (p._values[i] < q._values[j])
        probability += p._probabilities[i] * q._probabilities[j];
    }
  }
  return event<_Tp>(probability);
}

template <typename _Tp>
inline event<_Tp> operator==(const randvar<_Tp> &p, const randvar<_Tp> &q) {
  size_t i, j;
  double probability = 0.;
  for (i = 0; i < p._values.size(); ++i) {
    for (j = 0; j < q._values.size(); ++j) {
      if (p._values[i] == q._values[j])
        probability += p._probabilities[i] * q._probabilities[j];
    }
  }
  return event<_Tp>(probability);
}

/** Expected value of rv (not implement yet) */
template <typename _Tp> _Tp Expectation_of(const randvar<_Tp> &rv);

/** @} */
}
#endif // RANDVAR_HPP
