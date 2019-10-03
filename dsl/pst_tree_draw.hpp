#ifndef DSL_PST_TREE_DRAW_HPP
#define DSL_PST_TREE_DRAW_HPP

#include "pst_tree.hpp"
#include <map>
#include <svgstream.hpp>

namespace std {
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

static const int factor = 40;
static const int factor2 = 50;

// A first postorder traversal recursively computes for each
// vertex v, the horizontal displacement of the left and and right
// children of v with repect to v.
template <typename _Key, typename _Val>
void Displacement_Recur(
    const _Pst_tree_node<_Val, _Key> *v,
    std::map<const _Pst_tree_node<_Val, _Key> *, double> &dpm) {
  if (v->_M_left == 0 && v->_M_right == 0)
    return;

  typedef _Pst_tree_node<_Val, _Key> _Node;
  auto *lv = (_Node *)v->_M_left;
  auto *rv = (_Node *)v->_M_right;

  if (lv != 0) {
    Displacement_Recur(lv, dpm);
    if (rv == 0) {
      dpm[lv] = -factor;
      return;
    }
  }

  if (rv != 0) {
    Displacement_Recur(rv, dpm);
    if (lv == 0) {
      dpm[rv] = factor;
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
  double max_distance = 2 * factor;

  do {
    dpm_l += dpm[lv];
    dpm_r += dpm[rv];
    double distance = dpm_l - dpm_r + 2 * factor;
    if (max_distance < distance)
      max_distance = distance;
    lv = (_Node *)((lv->_M_right != 0) ? lv->_M_right : lv->_M_left);
    rv = (_Node *)((rv->_M_left != 0) ? rv->_M_left : rv->_M_right);
  } while (lv != 0 && rv != 0);

  dpm[(_Node *)v->_M_left] = -max_distance / 2;
  dpm[(_Node *)v->_M_right] = max_distance / 2;

  return;
}

// A second preorder traversal computes the
// x-coordinates of the vertices by accumulating the displacements
// on the path from each vertex to the root, and the y-coordinates
// of the vertices by determining the depth of each vertex.
template <typename _Key, typename _Val>
void Layered_Tree_Draw_Recur(
    svgstream &os, const _Pst_tree_node<_Val, _Key> *v,
    std::map<const _Pst_tree_node<_Val, _Key> *, double> &dpm, int depth) {
  typedef _Pst_tree_node<_Val, _Key> _Node;

  if (v->_M_left != 0) {
    dpm[(_Node *)v->_M_left] += dpm[v];
    // Recursively apply the algorithm to draw the left subtree of T.
    os << "  <line "
       << " x1=\"" << dpm[v] << "\""
       << " y1=\"" << depth << "\""
       << " x2=\"" << dpm[(_Node *)v->_M_left] << "\""
       << " y2=\"" << depth + factor2 << "\""
       << " style=\"stroke:black;\"/>\n";
    Layered_Tree_Draw_Recur(os, (const _Node *)v->_M_left, dpm,
                            depth + factor2);
  }
  if (v->_M_right != 0) {
    dpm[(_Node *)v->_M_right] += dpm[v];
    os << "  <line "
       << " x1=\"" << dpm[v] << "\""
       << " y1=\"" << depth << "\""
       << " x2=\"" << dpm[(const _Node *)v->_M_right] << "\""
       << " y2=\"" << depth + factor2 << "\""
       << " style=\"stroke:black;\"/>\n";
    // Recursively apply the algorithm to draw the right subtree of T.
    Layered_Tree_Draw_Recur(os, (const _Node *)v->_M_right, dpm,
                            depth + factor2);
  }

  if (v->_M_left == 0 && v->_M_right == 0) {
    os << "  <rect"
       << " x=\"" << dpm[v] - 20 << "\""
       << " y=\"" << depth - 20 << "\""
       << " width=\"40\" height=\"30\" style=\"fill:cyan;stroke:black;\"/>\n";
  } else {
    os << "  <ellipse"
       << " cx=\"" << dpm[v] << "\""
       << " cy=\"" << depth << "\""
       << " rx=\"20\" ry=\"15\" style=\"fill:yellow;stroke:black;\"/>\n";
  }

  os << "  <text text-anchor=\"middle\""
     << " x=\"" << dpm[v] << "\""
     << " y=\"" << depth << "\">" << v->_M_value_field << ","
     << v->_M_value_field2 << "</text>\n";
}

// xxx /** Information for the Layer_Tree_Draw algorithm */
// xxx struct Layer_Tree_Draw_Info
// xxx {
// xxx   vector<size_t>       _heights;
// xxx   slist<_Pst_tree_node<_Val, _Key>*> _left_contours;
// xxx   slist<_Pst_tree_node<_Val, _Key>*> _right_contours;
// xxx
// xxx   Layer_Tree_Draw_Info(int N) :
// xxx     heights(N), left_contours(N), right_contours(N) {}
// xxx   ~Layer_Tree_Draw_Info();
// xxx };

/**
 * Print a pstary tree in svg format. Layered-Tree-Draw is implemented
 * by means of two traversals of the input binary tree T. A first
 * postorder traversal recursively computes for each vertex v, the
 * horizontal displacement of the left and and right children of v
 * with repect to v. A second preorder traversal computes the
 * x-coordinates of the vertices by accumulating the displacements
 * on the path from each vertex to the root, and the y-coordinates
 * of the vertices by determining the depth of each vertex.
 * Time complexity is O(n).
 */
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
svgstream &
operator<<(svgstream &os,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &T) {
  typedef _Pst_tree_node<_Val, _Key> _Node;

  std::map<const _Node *, double> dpm;
  Displacement_Recur((_Node *)T._M_root(), dpm);
  dpm[(_Node *)T._M_root()] = 0;
  Layered_Tree_Draw_Recur(os, (_Node *)T._M_root(), dpm, 0);
  return os;
}

/**@} */
}

#endif
