#ifndef CGL_PT_STREE_HPP
#define CGL_PT_STREE_HPP

#ifndef RECTI_POINT_HPP
#include <point.hpp>
#endif
#ifndef DSL_STREE_HPP
#include <stree.hpp>
#endif

#include <iosfwd> // for declaration of std::istream and std::ostream

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */

/** A simple tree. Each node is a 2D point. */
class pt_stree : public dsl::stree<recti::point> {
  friend std::ostream &operator<<(std::ostream &os, const pt_stree &cs);

public:
  /** Create a tree with size n and random location. */
  pt_stree(size_t n) : dsl::stree<recti::point>(n) {
    for (size_t i = 0; i < num_nodes(); ++i) {
      _sa[i]._data = recti::point::random();
    }
  }

  /** Destructor */
  ~pt_stree() override = default;

  /**
   * Contruct a rectilinear minimum spanning tree using Prim's algorithm.
   * @return the total length (in Manhattan metric) of the MST.
   * Precondtions: Number of nodes > 1.
   * Time complexity: O(n^2). Space complexity: O(n).
   */
  double rmst() { return minimum_spanning_tree(); }
};
}

#endif
