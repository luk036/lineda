#include "robust_GP.hpp"
#include "posynomial.hpp"
#include <aa.h>
#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

using namespace std;

template <typename _Tp> robust_GP<_Tp>::robust_GP() {
  //_P.reserve(100);
}

template <typename _Tp> robust_GP<_Tp>::~robust_GP() {
  // xxx for (size_t i=0; i<_P.size(); ++i)
  // xxx   if (_P[i]) delete _P[i];
}

#include <aa.h>

// Specialization
// Robust GP reduces to the traditional GP if no variation present
template <> double robust_GP<double>::max(const double &c) const { return c; }

template <>
posynomial<double>
robust_GP<double>::convert(const posynomial<double> &c) const {
  return c;
}

template <>
template <>
std::valarray<double>
robust_GP<double>::convert(const std::valarray<double> &y) const {
  return y;
}

template <>
template <>
std::valarray<double>
robust_GP<AAF>::convert(const std::valarray<double> &y) const {
  return y;
}

template <> double robust_GP<AAF>::max(const AAF &c) const {
  return c.max(_Polarity);
}

template <>
posynomial<double> robust_GP<AAF>::convert(const posynomial<AAF> &c) const {
  return posynomial<double>(c, _Polarity);
}

template <>
template <>
std::valarray<double>
robust_GP<AAF>::convert(const std::valarray<AAF> &y) const {
  std::valarray<double> yd(y.size());
  for (size_t i = 0; i < y.size(); ++i) {
    yd[i] = y[i].eval(_Polarity);
  }
  return yd;
}

// Explicit instantiation
template class robust_GP<double>;
template class robust_GP<AAF>;
