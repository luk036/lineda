#include "bin_tree.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

using std::vector;

namespace dsl {

/* @return the cost between two nodes. Default is a unit cost. */
double bin_tree_base::cost(const _Node &, const _Node &) const { return 1.0; }

/** @return the total length cost. */
double bin_tree_base::total_cost() const {
  double res = 0.0;
  for (size_t i = 0; i < num_nodes(); ++i) {
    const _Node &s = get_node(i);
    if (s.parent() == nullptr)
      continue;
    res += cost(*s.parent(), s);
  }
  return res;
}

/* @return maximum (longest) path length (recursive function). */
double bin_tree_base::max_path_length(const _Node &t) const {
  const double left_length =
      (t._left == nullptr) ? 0.0
                           : max_path_length(*t._left) + cost(t, *t._left);
  const double right_length =
      (t._right == nullptr) ? 0.0
                            : max_path_length(*t._right) + cost(t, *t._right);
  return std::max(left_length, right_length);
}

/* @return minimum (shortest) path length (recursive function). */
double bin_tree_base::min_path_length(const _Node &t) const {
  const double left_length =
      (t._left == nullptr) ? 0.0
                           : min_path_length(*t._left) + cost(t, *t._left);
  const double right_length =
      (t._right == nullptr) ? 0.0
                            : min_path_length(*t._right) + cost(t, *t._right);
  return std::min(left_length, right_length);
}

/**
 * To generate a binary tree with n internal nodes and n+1 leaves,
 * Remy's algorithm proceeds as follows:
 *  o suppose that so far we have a binary tree with k internal nodes
 *    and k+1 leaves
 *  o randomly select one of the 2k+1 nodes; denote the selected node
 *    by v
 *  o replace v by a new node
 *  o choose v to be the left or right child of the new node with
 *    probability 1/2 for each of the alternatives; the other child of
 *    the new node is a new leaf (the subtree of v are kept unchanged)
 *  o the process of inserting nodes is repeated until the tree has n
 *    internal nodes and n+1 leaves
 * The correctness of Remy's algorithm can be proved by concerning
 * binary trees with leaves labelled by numbers 1,...,n+1. Namely, it
 * is easy to show that the algorithm generates all binary trees with
 * 2n+1 labelled leaves with probability n!/2n!.
 */
void bin_tree_base::randomize() {
  assert(_size % 2 || !"size is an odd number");

  /* the initialization of the tree */
  size_t root = 0;
  _bt_ab[0]->_left = nullptr;
  _bt_ab[0]->_right = nullptr;
  _bt_ab[0]->_parent = nullptr;

  /* the "induction step" */
  for (size_t i = 1; i < _size; i += 2) {
    _Node *const hit = _bt_ab[rand() % i]; // the choice of v
    const int direction = rand() & 1;      // the choice of the order of
                                           // the children of the new node
    _Node *const parent = hit->_parent;
    if (parent == nullptr)
      root = i;
    else if (parent->_left == hit)
      parent->_left = _bt_ab[i];
    else
      parent->_right = _bt_ab[i];

    _bt_ab[i]->_parent = parent;
    if (direction == 0) {
      _bt_ab[i]->_left = _bt_ab[i + 1];
      _bt_ab[i]->_right = hit;
    } else {
      _bt_ab[i]->_left = hit;
      _bt_ab[i]->_right = _bt_ab[i + 1];
    }

    /* insert of the new leaf into the position i + 1 */
    hit->_parent = _bt_ab[i];
    _bt_ab[i + 1]->_left = nullptr;
    _bt_ab[i + 1]->_right = nullptr;
    _bt_ab[i + 1]->_parent = _bt_ab[i];
  }

  _root = _bt_ab[root];
  assert(is_valid());
}

/**
 * To generate a slicing tree with n internal nodes and n+1 leaves,
 * Modified Remy's algorithm proceeds as follows:
 *  o suppose that so far we have a binary tree with k internal nodes
 *    and k+1 leaves
 *  o randomly select one of the 2k+1 nodes according to appropriate
 *    probability; denote the selected node by v
 *  o replace v by a new node
 *  o choose v to be the left or right child of the new node with
 *    appropriate probability for each of the alternatives;
 *    the other child of
 *    the new node is a new leaf (the subtree of v are kept unchanged)
 *  o the process of inserting nodes is repeated until the tree has n
 *    internal nodes and n+1 leaves.
 */
void bin_tree_base::randomize_st() {
  assert(_size % 2 || !"size is an odd number");

  /* the initialization of the tree */
  size_t root = 0;
  _bt_ab[0]->_left = nullptr;
  _bt_ab[0]->_right = nullptr;
  _bt_ab[0]->_parent = nullptr;

  if (_size == 1) {
    _root = _bt_ab[0];
    return;
  }

  vector<_Node *> internal_nodes;
  vector<_Node *> right_nodes;
  vector<_Node *> left_nodes;

  internal_nodes.reserve(_size / 2);
  left_nodes.reserve(_size / 2);
  right_nodes.reserve(_size / 2);

  left_nodes.push_back(_bt_ab[0]);

  size_t k = 1; // number of left leave nodes

  /* the "induction step" */
  for (size_t i = 1; i < _size; i += 2) {
    const size_t n = i / 2;
    const size_t num_l = 2 * k;
    const size_t num_i = 3 * n;
    const size_t num_r = 4 * (n + 1 - k);
    size_t c = std::rand() % (num_i + num_l + num_r);

    int direction = 0; // the choice of the order of
    // the children of the new node

    internal_nodes.push_back(_bt_ab[i]);

    _Node *hit = nullptr;

    // Choose v according to the probabilities
    if (c < num_i) {
      hit = internal_nodes[c / 3];
      // xxx hit = _bt_ab[2 * (rand() % n) + 1];  // internal nodes have odd
      // numbers
      direction = c % 3;
      if (direction != 0) {
        left_nodes.push_back(_bt_ab[i + 1]);
        ++k;
      } else {
        right_nodes.push_back(_bt_ab[i + 1]);
      }
    } else {
      c -= num_i;
      if (c < num_r) {
        hit = right_nodes[c / 4];
        left_nodes.push_back(_bt_ab[i + 1]);
        ++k;
        direction = 1;
      } else {
        c -= num_r;
        hit = left_nodes[c / 2];
        right_nodes.push_back(_bt_ab[i + 1]);
        direction = 0;
      }
    }

    _Node *const parent = hit->_parent;
    if (parent == nullptr)
      root = i;
    else if (parent->_left == hit)
      parent->_left = _bt_ab[i];
    else
      parent->_right = _bt_ab[i];

    _bt_ab[i]->_parent = parent;

    if (direction == 0) {
      _bt_ab[i]->_left = hit;
      _bt_ab[i]->_right = _bt_ab[i + 1];
    } else {
      _bt_ab[i]->_left = _bt_ab[i + 1];
      _bt_ab[i]->_right = hit;
    }

    /* insert of the new leaf into the position i + 1 */
    hit->_parent = _bt_ab[i];
    _bt_ab[i + 1]->_left = nullptr;
    _bt_ab[i + 1]->_right = nullptr;
    _bt_ab[i + 1]->_parent = _bt_ab[i];
  }

  _root = _bt_ab[root];
  assert(is_valid());
}

/** Destructor */
bin_gen_tree::~bin_gen_tree() { delete[] _bgt_a; }

/** Create a tree consists of a right path of n nodes.
    Reference: J. Lucas, D. Roelants van Baronaigien and F. Ruskey,
    Generating Binary Trees by Rotations, J. Algorithms, 15 (1993)
    343-366.
    Precondtion: n > 1. */
bin_gen_tree::bin_gen_tree(size_t n)
    : bin_tree_base(n), _bgt_a(new _Node[n]), _uhead(nullptr), _fhead(nullptr),
      _rank(0) {
  size_t i;

  if (n == 0)
    return;

  _Node *s = _bgt_a;
  for (i = 0; i < n; ++i) {
    _bt_ab[i] = s++;
  }

  _Node *tree = _bgt_a;
  _root = tree;

  tree->_parent = nullptr;
  tree->_left = nullptr;
  tree->_direct = true;
  tree->_next = nullptr;

  if (n == 1) {
    tree->_right = nullptr;
    return;
  }

  s = tree;
  for (i = 1; i < n; ++i) {
    s->_right = _bgt_a + i;
    auto *r = (_Node *)s->_right;
    r->_parent = s;
    r->_next = s;
    s = r;
    s->_left = nullptr;
    s->_direct = true;
  }
  s->_right = nullptr;

  // Unfinished list consists of all but first node
  _fhead = tree;
  _uhead = s;
  ((_Node *)tree->_right)->_next = nullptr;
}

/** Reference: J. Lucas, D. Roelants van Baronaigien and F. Ruskey,
    Generating Binary Trees by Rotations, J. Algorithms, 15 (1993)
    343-366. */
void bin_gen_tree::create_next_tree() {
  _Node *current = _uhead; // next node to be rotated

  // Nodes > current are added to unfinished list
  if (_uhead != _bgt_a + num_nodes() - 1) {
    _uhead = _fhead;
    _fhead = (current + 1)->_next;
    (current + 1)->_next = current;
  }

  if (current->is_up())
    rotate_left(current->_parent);
  else
    rotate_right(current);

  // If current node has finished rotating in its direction, move it
  // to the front of the list of finished nodes and change its
  // direction.
  if (current->done_in_direction()) {
    if (current == _uhead)
      _uhead = current->_next;
    else
      (current + 1)->_next = current->_next;
    current->_next = _fhead;
    _fhead = current;
    current->_direct = !current->_direct;
  }

  _rank++;
}

/** Print out the tree structure in .dot format to os.
    (Recursive function) */
void bin_gen_tree::print_it(std::ostream &os, const _Node_Base *t,
                            size_t &d) const {
  if (t->left() != nullptr) {
    os << "  n" << rank() << t << " -> n" << rank() << t->left()
       << " [label=left];\n";
    print_it(os, t->left(), d);
  } else {
    os << "  n" << rank() << t << " -> n" << rank() << _bgt_a + d++
       << " [label=left];\n";
  }

  if (t->right() != nullptr) {
    os << "  n" << rank() << t << " -> n" << rank() << t->right()
       << " [label=right];\n";
    print_it(os, t->right(), d);
  } else {
    os << "  n" << rank() << t << " -> n" << rank() << _bgt_a + d++
       << " [label=right];\n";
  }
}

/** Print out the tree structure in .dot format to os */
std::ostream &operator<<(std::ostream &os, const bin_gen_tree &tree) {
  size_t i;
  size_t d = tree.num_nodes();

  os << "subgraph g" << tree.rank() << " {\n"
     << "  node [shape=circle];\n";

  for (i = 0; i < d; ++i) {
    os << "  n" << tree.rank() << tree._bgt_a + i << " [label=\"" << i
       << "\"];\n";
  }

  for (; i < 2 * d + 1; ++i) {
    os << "  n" << tree.rank() << tree._bgt_a + i
       << " [shape=plaintext label=null];\n";
  }

  if (d != 0)
    tree.print_it(os, tree._root, d);

  os << "}\n";
  os << std::endl;

  return os;
}

// Explicit instantiations
template class bin_tree<int>;
};
