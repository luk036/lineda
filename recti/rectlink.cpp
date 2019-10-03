#include "rectlink.hpp"
#include <boost/static_assert.hpp>
#include <interval.hpp>
#include <svgstream.hpp>

using numeric::interval;
using std::max;
using std::min;
using std::ostream;

namespace recti {

/** Create a rectangle with lower left point, width and height.
    Precondition: width and height are non-negative. */
template <typename _Tp>
Rectlink<_Tp>::Rectlink(const Pointer<_Tp> &ll, const _Tp &width,
                        const _Tp &height)
    : _Base(interval<_Tp>(ll.x(), ll.x() + width),
            interval<_Tp>(ll.y(), ll.y() + height)) {
  // xxx assert(!(width < 0 || height < 0));
}

/** Explicit instantiation */
template class Rectlink<int>;
template class Rectlink<interval<int>>;
// xxx template ostream& operator<<(ostream& os, const Rectangle<int>& r);
// xxx template ostream& operator<<(ostream& os, const Rectangle<interval<int>
// >& r);
}
