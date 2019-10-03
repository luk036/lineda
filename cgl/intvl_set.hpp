#ifndef CGL_INTVL_SET_HPP
#define CGL_INTVL_SET_HPP

#ifndef NUMERIC_INTERVAL_HPP
#include <interval.hpp>
#endif

#include <functional>
#include <set>

/**
 * @defgroup cgl Computational Geometry Libary
 * @ingroup lineda
 *
 * This module provides obbjects such as range tree, kd-tree,
 * quad-tree, voronoi diagram, visiblity graphs. Provides operations
 * such as find_nearest_neighbor(), 2D_range_search().
 */

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */

/** Left_of. Function adpator */
template <class _Tp>
struct left_of : public std::binary_function<_Tp, _Tp, bool> {
  /** @return whether x is left of y */
  bool operator()(const _Tp &__x, const _Tp &__y) const {
    return __x.left_of(__y);
  }
};

/** Interval set for overlap detection */
template <class _Tp>
class intvl_set
    : public std::set<numeric::interval<_Tp>, left_of<numeric::interval<_Tp>>> {
  typedef std::set<numeric::interval<_Tp>, left_of<numeric::interval<_Tp>>>
      _Base;

public:
  typedef typename _Base::iterator iterator;
  typedef typename _Base::const_iterator const_iterator;

  /** Default constructor */
  intvl_set() : _Base() {}

  /** Destructor */
  ~intvl_set() = default;

  /** @return whether this interval set overlaps with interval q.
      Time complexity: O(log n) */
  bool overlap(const numeric::interval<_Tp> &q) const {
    if (_Base::find(q) == _Base::end())
      return false;
    return true;
  }
};

/**@} */
}

#endif
