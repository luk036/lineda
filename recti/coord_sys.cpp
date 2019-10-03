#include "coord_sys.hpp"
#include <boost/static_assert.hpp>
#include <interval.hpp>
#include <iostream>

using numeric::interval;

namespace recti {

// Don't increase the size.
BOOST_STATIC_ASSERT(sizeof(coord_sys) <= 12);

template <> coord_sys coord_sys::_identity = coord_sys();

/** Explicit instantiation */
template class Coord_sys<int>;
template class Coord_sys<interval<int>>;
}
