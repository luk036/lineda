#ifndef DSL_BIN_TREE_DRAW_HPP
#define DSL_BIN_TREE_DRAW_HPP

#include "bin_tree.hpp"
#include <svgstream.hpp>
#include <vector>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/**
 * Reference:
 *   G. D. Battista, P. Eades, R. Tamassia and I. G. Tollis,
 *   Graph Drawing: Algorithms for the visualization of graphs,
 *   Chapter 3, Algorithm 3.1
 */
static const int factor = 100;

// A first postorder traversal recursively computes for each
// vertex v, the horizontal displacement of the left and and right
// children of v with repect to v.
template <typename _Tp>
void Displacement_Recur(const bt_node<_Tp> *v, const bin_tree<_Tp> &T,
                        std::vector<double> &dpm) {
  if (v->is_leaf())
    return;

  bt_node<_Tp> *lv = v->_left;
  bt_node<_Tp> *rv = v->_right;

  if (lv != 0) {
    Displacement_Recur(lv, T, dpm);
    if (rv == 0) {
      dpm[T.get_id(lv)] = -factor;
      return;
    }
  }

  if (rv != 0) {
    Displacement_Recur(rv, T, dpm);
    if (lv == 0) {
      dpm[T.get_id(rv)] = factor;
      return;
    }
  }

  // Conquer: Imagine the each subtree is drawn on a separate sheet
  // of paper. Move the drawings of the subtrees towards each other
  // until their horizontal distance becomes equal to 2. Finally,
  // place the root r of T vertically one unit above and
  // horizontally half way between its children. If r has only one
  // subtree, say the left one, then place r at horizontal distance
  // 1 to the right of its left child.

  double dpm_l = 0;
  double dpm_r = 0;
  double max_distance = 200;

  do {
    dpm_l += dpm[T.get_id(lv)];
    dpm_r += dpm[T.get_id(rv)];
    double distance = dpm_l - dpm_r + 200;
    if (max_distance < distance)
      max_distance = distance;
    lv = (lv->_right != 0) ? lv->_right : lv->_left;
    rv = (rv->_left != 0) ? rv->_left : rv->_right;
  } while (lv != 0 && rv != 0);

  dpm[T.get_id(v->_left)] = -max_distance / 2;
  dpm[T.get_id(v->_right)] = max_distance / 2;

  return;
}

// A second preorder traversal computes the
// x-coordinates of the vertices by accumulating the displacements
// on the path from each vertex to the root, and the y-coordinates
// of the vertices by determining the depth of each vertex.
template <typename _Tp>
void Layered_Tree_Draw_Recur(svgstream &os, const bt_node<_Tp> *v,
                             const bin_tree<_Tp> &T, std::vector<double> &dpm,
                             int depth) {
  size_t id = T.get_id(v);
  if (v->_parent != 0) {
    size_t idp = T.get_id(v->_parent);
    os << "  <line "
       << " x1=\"" << dpm[id] << "\""
       << " y1=\"" << depth << "\""
       << " x2=\"" << dpm[idp] << "\""
       << " y2=\"" << depth - factor << "\""
       << " r=\"20\" style=\"fill:yellow;stroke:black;\"/>\n";
  }

  if (v->_left != 0) {
    dpm[T.get_id(v->_left)] += dpm[id];
    // Recursively apply the algorithm to draw the left subtree of T.
    os << "  <line "
       << " x1=\"" << dpm[id] << "\""
       << " y1=\"" << depth << "\""
       << " x2=\"" << dpm[T.get_id(v->_left)] << "\""
       << " y2=\"" << depth + factor << "\""
       << " style=\"stroke:black;\"/>\n";
    Layered_Tree_Draw_Recur(os, v->_left, T, dpm, depth + factor);
  }
  if (v->_right != 0) {
    dpm[T.get_id(v->_right)] += dpm[id];
    os << "  <line "
       << " x1=\"" << dpm[id] << "\""
       << " y1=\"" << depth << "\""
       << " x2=\"" << dpm[T.get_id(v->_right)] << "\""
       << " y2=\"" << depth + factor << "\""
       << " style=\"stroke:black;\"/>\n";
    // Recursively apply the algorithm to draw the right subtree of T.
    Layered_Tree_Draw_Recur(os, v->_right, T, dpm, depth + factor);
  }

  os << "  <circle"
     << " cx=\"" << dpm[id] << "\""
     << " cy=\"" << depth << "\""
     << " r=\"20\" style=\"fill:yellow;stroke:black;\"/>\n";

  os << "  <text text-anchor=\"middle\""
     << " x=\"" << dpm[id] << "\""
     << " y=\"" << depth << "\">"
     << "n" << id << "</text>\n";
}

// xxx /** Information for the Layer_Tree_Draw algorithm */
// xxx struct Layer_Tree_Draw_Info
// xxx {
// xxx   vector<size_t>       _heights;
// xxx   slist<bt_node<_Tp>*> _left_contours;
// xxx   slist<bt_node<_Tp>*> _right_contours;
// xxx
// xxx   Layer_Tree_Draw_Info(int N) :
// xxx     heights(N), left_contours(N), right_contours(N) {}
// xxx   ~Layer_Tree_Draw_Info();
// xxx };

/**
 * Print a binary tree in svg format. Layered-Tree-Draw is implemented
 * by means of two traversals of the input binary tree T. A first
 * postorder traversal recursively computes for each vertex v, the
 * horizontal displacement of the left and and right children of v
 * with repect to v. A second preorder traversal computes the
 * x-coordinates of the vertices by accumulating the displacements
 * on the path from each vertex to the root, and the y-coordinates
 * of the vertices by determining the depth of each vertex.
 * Time complexity is O(n).
 */
template <typename _Tp>
svgstream &operator<<(svgstream &os, const bin_tree<_Tp> &T) {
  // xxx Layer_Tree_Draw_Info info(T.num_nodes());
  std::vector<double> dpm(T.num_nodes());
  Displacement_Recur((bt_node<_Tp> *)T.root(), T, dpm);
  dpm[T.get_id((bt_node<_Tp> *)T.root())] = 0;
  Layered_Tree_Draw_Recur(os, (bt_node<_Tp> *)T.root(), T, dpm, 0);

  // xxx for (size_t i=0; i<T.num_nodes(); ++i) {
  // xxx   const typename bin_tree<_Tp>::_Node& s = T.get_node(i);
  // xxx   if (s.is_root()) continue;
  // xxx   os << "  \"n" << i << "\" -> \"n" << T.get_id(s.parent()) << "\"\n";
  // xxx }
  return os;
}

// xxx /** Print the point in SVG format.*/
// xxx template<typename _Key, typename _Val, typename _KeyOfValue,
// xxx          typename _Compare, typename _Alloc>
// xxx ostream& Layered_Tree_Draw
// xxx (ostream& osvgs,
// xxx  const _Pst_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>& pst)
// xxx {
// xxx   _Print_recur(osvgs, -10000, 10000, pst._M_root());
// xxx   return osvgs;
// xxx }

/**@} */
}

#endif
