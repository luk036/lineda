#ifndef DSL_STREE_HPP
#define DSL_STREE_HPP

#include "sl_base.hpp"
#include "snode.hpp"
#include <iosfwd> // for declaration of std::istream and std::ostream

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** A tree data structure using singly linked list */
template <class _Tp> class stree : public slist_base<_Tp> {
  typedef slist_base<_Tp> _Base;

public:
  /** Create a tree with size n */
  stree(size_t n) : slist_base<_Tp>(n) { slist_base<_Tp>::make_tree(); }

  /** Destructor */
  ~stree() override = default;
};

/** Print out the tree structure in .dot format to os */
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &os, const stree<_Tp> &cs) {
  return cs.print_tree(os);
}

/** @} */
}

#endif
