#include "posynomial.hpp"
#include <aa.h>

/** Constructor (for AAF -> double) */
template <>
template <>
posynomial<double>::posynomial(const posynomial<AAF> &posyn,
                               const AAF &polarity)
    : _M(posyn._M.size(), monomial<double>(posyn._M[0]._a.size())),
      _p(posyn._p.size()) {
  for (size_t i = 0; i < _M.size(); ++i) {
    _M[i] = monomial<double>(posyn._M[i], polarity);
  }
  for (size_t j = 0; j < _p.size(); ++j) {
    _p[j] = posyn._p[j].eval(polarity);
  }
  assert(_M.size() == _p.size());
}

// Explicit instantiation
template class posynomial<double>;
template class posynomial<AAF>;
