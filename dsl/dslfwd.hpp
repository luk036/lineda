#ifndef DSL_FWD_HPP
#define DSL_FWD_HPP

/** Forward declarations */

namespace dsl {
class bad_bt_tree;
class bt_node_base;
class bgt_node;
class bin_tree_base;
class bin_gen_tree;
template <typename Tp> class bt_node;
template <typename Tp> class bin_tree;
template <class _Node> class dlist;
template <class _Ref, class _Ptr> class dnode_base_iterator;
class dnode_base;
template <typename _Tp> class dnode;
template <typename _Tp, typename _Ref, typename _Ptr> class dnode_iterator;
class bpq_node;
template <class _Node> class gbpqueue;
template <class _Node> class gbpqueueIterator;
template <class _Node> class gdlist;
template <class _Node> class glist;
template <class node> class listCursor;
template <class node> class listConstCursor;
template <class node> class GBPQueue;
template <class node> class GBPQueue2;
template <class node> class list template <typename _Tp> class sclist;
class sl_base;
template <typename _Tp> class slist_base;
class non_canonical_flrpln;
class slicing_flrpln;
template <typename _Tp> class slist;
template <class _Ref, class _Ptr> class snode_base_iterator;
struct snode_base;
template <typename _Tp, typename _Ref, typename _Ptr> class snode_iterator;
template <typename _Tp> struct snode;
template <class _Tp> class stree;
class bad_tri_tree;
class tri_tree_base;
template <typename _Tp> class tri_tree;
struct tt_node_base;
}

#endif
