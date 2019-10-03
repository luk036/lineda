#ifndef DSL_TRI_TREE_HPP
#define DSL_TRI_TREE_HPP

#ifndef DSL_TT_NODE_HPP
#include "tt_node.hpp"
#endif
#ifndef DSL_STREE_HPP
#include "stree.hpp"
#endif

#include <iosfwd>    // for declaration of std::istream and std::ostream
#include <stdexcept> // for std::domain_error

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Class that will be thrown when invalid tri-nary tee node is found */
class bad_tri_tree : public std::domain_error {
public:
  explicit bad_tri_tree() : std::domain_error("bad tri_tree") {}
};

/** A tri-nary tree data structure. */
class tri_tree_base : public sl_base {
protected:
  /** Create a tree with size n. Precondtion: n > 1. */
  tri_tree_base(size_t n)
      : sl_base(n), _tri_ab((tt_node_base **)_sab), _root(nullptr) {
    assert(n > 1);
  }

  /** Destructor */
  ~tri_tree_base() override = default;

  /** @return whether it is a valid tree. */
  inline bool is_valid() const;

  /**
   * Contruct a minimum spanning tree using Prim's algorithm.
   * @return the total distance (cost) of the MST.
   * Pre-condtions:
   *   1. The maximum node degree of the MST <= 4.
   *   2. Number of nodes > 1.
   *   3. cost() is symmetric, i.e., cost(a, b) == cost(b, a).
   *      However, it could be negative value and doesn't need to satisfy
   *      the triangle inequality: cost(a, c) <= cost(a, b) + cost(b, c).
   * Post-condition: total distance of MST <= total distance of initial tree
   * Time complexity: O(n^2). Space complexity: O(n).
   */
  double mst(size_t i = 0);

private:
  // Unimplement
  tri_tree_base(const tri_tree_base &) = delete;
  tri_tree_base &operator=(const tri_tree_base &) = delete;

  void check_valid_recur(const tt_node_base *root,
                         const tt_node_base *child) const;

protected:
  tt_node_base **const _tri_ab; /**< array of pointer of node */
  tt_node_base *_root;          /**< root of the tree */
};

/** @return whether it is a valid tree. */
inline bool tri_tree_base::is_valid() const {
  try {
    check_valid_recur(nullptr, _root);
    return true;
  } catch (bad_tri_tree &) {
    return false;
  }
}

/** A tri-nary tree data structure. */
template <typename _Tp> class tri_tree : public tri_tree_base {
public:
  /** Create a tree with size n. Precondtion: n > 1. */
  tri_tree(size_t n) : tri_tree_base(n), _tri_a(new tt_node<_Tp>[n]) {
    assert(n > 1);
    tt_node<_Tp> *s = _tri_a;
    for (size_t i = 0; i < n; ++i) {
      _tri_ab[i] = (tt_node_base *)s++;
    }
  }

  /** Destructor */
  ~tri_tree() override { delete[] _tri_a; }

  /**
   * @return the total cost of the tree.
   * (Note: this is actually a non-inlined function because it is virtual.)
   */
  double cost(const snode_base *, const snode_base *) const override {
    return 1.0;
    // xxx return (double) distance(((const tt_node<_Tp>*) a)->_data,
    // xxx                 ((const tt_node<_Tp>*) b)->_data);
  }

private:
  // Unimplement
  tri_tree(const tri_tree<_Tp> &) = delete;
  tri_tree<_Tp> &operator=(const tri_tree<_Tp> &) = delete;

protected:
  tt_node<_Tp> *const _tri_a; /**< array of pointer of node */
};

/** @} */
}

#endif
