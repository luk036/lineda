#ifndef CGL_PT_TRI_TREE_HPP
#define CGL_PT_TRI_TREE_HPP

#ifndef RECTI_POINT_HPP
#include <point.hpp>
#endif
#ifndef DSL_TRI_TREE_HPP
#include <tri_tree.hpp>
#endif

#include <iosfwd> // for declaration of std::istream and std::ostream

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */

/** A simple tree. Each node is a 2D point. */
class pt_tri_tree : public dsl::tri_tree<recti::point> {
  friend std::ostream &operator<<(std::ostream &os, const pt_tri_tree &cs);

public:
  /** Create a tree with size n and random location. */
  pt_tri_tree(size_t n) : dsl::tri_tree<recti::point>(n) {
    for (size_t i = 0; i < num_nodes(); ++i) {
      _tri_a[i]._data = recti::point::random();
    }
  }

  /** Destructor */
  ~pt_tri_tree() override = default;

  /**
   * Contruct a rectilinear minimum spanning tree using Prim's algorithm.
   * @return the total length (in Manhattan metric) of the MST.
   * Precondtions: Number of nodes > 1.
   * Time complexity: O(n^2). Space complexity: O(n).
   */
  int rmst(size_t idx = 0) { return (int)mst(idx); }
};

/**@} */
}

#endif
