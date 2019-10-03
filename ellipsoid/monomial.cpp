#include "monomial.hpp"
#include <aa.h>

/** Constructor (for AAF -> double) */
template <>
template <>
monomial<double>::monomial(const monomial<AAF> &mon, const AAF &polarity)
    : _a(mon._a), _b(mon._b.eval(polarity)) {}

template class monomial<double>;
template class monomial<AAF>;
