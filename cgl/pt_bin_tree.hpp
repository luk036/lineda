#ifndef CGL_PT_BIN_TREE_HPP
#define CGL_PT_BIN_TREE_HPP

#ifndef RECTI_POINT_HPP
#include <point.hpp>
#endif
#ifndef DSL_BIN_TREE_HPP
#include <bin_tree.hpp>
#endif

class svgstream;

namespace cgl {
/**
 * @addtogroup cgl
 * @{
 */

/** A simple tree. Each node is a 2D point. */
class pt_bin_tree : public dsl::bin_tree<recti::point> {
  friend svgstream &operator<<(svgstream &os, const pt_bin_tree &cs);

public:
  /** Create a tree with size n and random location. */
  pt_bin_tree(size_t n) : dsl::bin_tree<recti::point>(n) {}

  /** Destructor */
  ~pt_bin_tree() override = default;

  /** @return the cost between two points. */
  double cost(const dsl::bt_node_base &a,
              const dsl::bt_node_base &b) const override;
};

/**@} */
}

#endif
