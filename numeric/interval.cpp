#include "interval.hpp"
#include <algorithm>

// 	$Id: interval.cpp 891 2006-03-25 06:17:03Z luk $
using std::max;
using std::min;

namespace numeric {

/** Create a random interval. */
template <typename _Tp> interval<_Tp> interval<_Tp>::random() {
  _Tp lower = _Tp::random();
  _Tp upper = _Tp::random();
  if (upper < lower)
    std::swap(lower, upper);
  return _Self(lower, upper);
}

/** Create a random interval (Specialized for int). */
template <> interval<int> interval<int>::random() {
  int lower = (rand() - rand()) % 1000;
  int upper = (rand() - rand()) % 1000;
  if (upper < lower)
    std::swap(lower, upper);
  return interval<int>(lower, upper);
}

/** Create a random interval (Specialized for unsigned int). */
template <> interval<unsigned int> interval<unsigned int>::random() {
  unsigned int lower = rand() % 1000;
  unsigned int upper = rand() % 1000;
  if (upper < lower)
    std::swap(lower, upper);
  return interval<unsigned int>(lower, upper);
}

// Explicit instantiations
template class interval<int>;
template class interval<interval<int> >;
}
