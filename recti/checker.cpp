#include "checker.hpp"
#include "path.hpp"
#include "point.hpp"
#include "polygon.hpp"
#include "rectangle.hpp"

namespace recti {
/** Explicit instantiations */
template class checker<point>;
template class checker<rectangle>;
template class checker<polygon>;
template class checker<path>;
}
