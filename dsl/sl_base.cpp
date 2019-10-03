#include "sl_base.hpp"
#include <cassert>
#include <cfloat> // for DBL_MAX
#include <iostream>
#include <vector>

using std::vector;

namespace dsl {
/** Create a singly linked list. */
sl_base::sl_base(size_t n) : _size(n), _sab(new node *[n]) { assert(n > 0); }

/** Destructor */
sl_base::~sl_base() { delete[] _sab; }

/** @return the cost between two nodes. Default is a unit cost. */
double sl_base::cost(const snode_base *, const snode_base *) const {
  return 1.0;
}

/**
 * Contruct a minimum spanning tree using Prim's algorithm.
 * @return the total cost of the MST.
 * Pre-condtions:
 *   1. Number of nodes > 1.
 *   2. cost() is symmetric, i.e., cost(a, b) == cost(b, a).
 *      However, it could be negative value and doesn't need to satisfy
 *      the triangle inequality: cost(a, c) <= cost(a, b) + cost(b, c).
 * Post-condition:
 *   1. Node 0 is the root of the tree.
 * Time complexity: O(n^2).
 * Space complexity: O(n) [*note*: not O(n^2)]
 */
double sl_base::minimum_spanning_tree() {
  make_tree(); // initialize a tree: each node points to node 0
  if (num_nodes() == 1)
    return 0;
  if (num_nodes() == 2)
    return cost(_sab[0], _sab[1]);

  vector<double> min_cost(num_nodes());        // minimum cost for each node
  vector<bool> is_visited(num_nodes(), false); // mark if the node is in MST
  int min_id = 1; // node id of the node with min. cost
  double min_min_cost = cost(_sab[0], _sab[1]);
  double total_init_cost = min_min_cost;

  min_cost[1] = min_min_cost;

  size_t i, j;

  for (j = 2; j < num_nodes(); ++j) { // initialize the costs for each node
    double dist = cost(_sab[0], _sab[j]);
    assert(dist == cost(_sab[j], _sab[0])); // cost function is symmetric
    min_cost[j] = dist;
    total_init_cost += dist;
    // sa[j]._next = sa;

    if (min_min_cost > dist) {
      min_min_cost = dist;
      min_id = j;
    }
  }

  node *s = _sab[min_id];
  is_visited[min_id] = true;
  double total_mst_cost = min_min_cost;

  for (i = 2; i < num_nodes(); ++i) {
    double min_min_cost = DBL_MAX;
    int min_id = -1; // node id of the node with min. cost

    for (j = 1; j < num_nodes(); ++j) {
      if (is_visited[j])
        continue;

      double dist = cost(s, _sab[j]);
      assert(dist == cost(_sab[j], s)); // cost function is symmetric
      if (min_cost[j] > dist) {
        min_cost[j] = dist;
        _sab[j]->_next = s;
      }
      if (min_min_cost > min_cost[j]) {
        min_min_cost = min_cost[j];
        min_id = j;
      }
    }
    s = _sab[min_id];
    is_visited[min_id] = true;
    total_mst_cost += min_min_cost;
  }

  // Assert: total cost of MST <= total cost of initial tree
  assert(total_mst_cost <= total_init_cost);
  return total_mst_cost;
}

/** Print out the tree structure in .dot format to os */
std::ostream &sl_base::print_tree(std::ostream &os) const {
  size_t i;

  os << "digraph g {\n"
     << "  node [shape=circle];\n"
     << "  n0 [shape=plaintext label=null];\n";
  for (i = 0; i < num_nodes(); ++i) {
    os << "  n" << _sab[i] << " [label=\"" << i << "\"];\n";
  }

  for (i = 0; i < num_nodes(); ++i) {
    os << "  n" << _sab[i] << " -> n" << _sab[i]->_next << ";\n";
  }
  os << "}\n";

  return os;
}

/** Print out the structure in .dot format to os */
std::ostream &sl_base::print_clist(std::ostream &os,
                                   const snode_base *tail) const {
  size_t i;

  assert(tail->loop_back());

  os << "digraph g {\n"
     << "  node [shape=circle]\n";
  for (i = 0; i < num_nodes(); ++i) {
    os << "  n" << _sab[i] << " [label=\"" << i + 1 << "\"];\n";
  }

  os << "  tail [shape=plaintext];\n";
  os << "  tail -> n" << tail << ";\n";

  snode_base::const_iterator c1(tail->_next);
  snode_base::const_iterator c2(c1);

  do {
    os << "  n" << (c1++).value();
    os << " -> n" << c1.value() << ";\n";
  } while (c1 != c2);

  return os;
}

// Explicit instantiations
template class slist_base<int>;
}
