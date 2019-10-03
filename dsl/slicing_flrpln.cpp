#include "slicing_flrpln.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

using std::vector;

namespace dsl {

/** Destructor */
slicing_flrpln::~slicing_flrpln() { delete[] _bgt_a; }

/** Create a flrpln consists of a right path of n nodes.
    Precondtion: n > 0. */
slicing_flrpln::slicing_flrpln(size_t n)
    : _Base(2 * n - 1), _bgt_a(new _Node[2 * n - 1]) {
  assert(n > 0);
  _Node *s = _bgt_a;
  for (size_t i = 0; i < _size; ++i) {
    _bt_ab[i] = s++;
  }
}

void slicing_flrpln::rand_dir(_Node *t) {
  if (t->is_leaf())
    return;
  rand_dir((_Node *)t->_left);
  rand_dir((_Node *)t->_right);
  if (t->_left->is_leaf())
    t->_direct = std::rand() & 1;
  else
    t->_direct = !((_Node *)t->_left)->_direct;
}

void slicing_flrpln::randomize() {
  _Base::randomize_st();
  rand_dir((_Node *)_root);
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
 *
 * Reference:
 * [1] D. Knuth. The Art of Computer Programming vol 4 Pre-Fascicle 4A.
 *     Section 7.2.1.6 Generating All Trees
 */
// xxx void slicing_flrpln::randomize()
// xxx {
// xxx   assert(_size % 2 || !"size is an odd number");
// xxx
// xxx   vector<size_t> Si;
// xxx   vector<size_t> Sr;
// xxx   vector<size_t> Sl;
// xxx
// xxx   Si.reserve(_size/2);
// xxx   Sl.reserve(_size/2);
// xxx   Sr.reserve(_size/2);
// xxx
// xxx   Sl.push_back(0);
// xxx
// xxx   _bt_ab[0] = (_Node_Base*) _bgt_a;     // L(0) <- 0
// xxx   size_t i = 1;                         // n <- 0
// xxx   size_t l = 1;                         // number of left leave nodes
// xxx
// xxx   for (; i<_size; i+=2) {               // terminate the algorithm if n =
// N
// xxx     const size_t n = i / 2;
// xxx     const size_t num_l = 2*l;
// xxx     const size_t num_i = 3*n;
// xxx     const size_t num_r = 4*(n+1-l);
// xxx     const size_t c = rand() % (num_i + num_l + num_r);
// xxx     size_t b=0;
// xxx     size_t k=0;
// xxx
// xxx     // Choose v according to the probabilities
// xxx     if (c < num_i) {
// xxx       k = Si[rand() % n];
// xxx       const size_t dir = rand()%3;
// xxx       if (dir == 0) {
// xxx         Sr.push_back(i+1);
// xxx         b = 0;
// xxx       }
// xxx       else {
// xxx         Sl.push_back(i);
// xxx 	  b = 1;
// xxx         ++l;
// xxx       }
// xxx     }
// xxx     else {
// xxx       if (c < num_i+num_r) {
// xxx         k = Sr[rand() % (n+1-l)];
// xxx         Sl.push_back(i);
// xxx         b = 1;
// xxx         ++l;
// xxx       }
// xxx       else {
// xxx         k = Sl[rand() % l];
// xxx         Sr.push_back(i+1);
// xxx         b = 0;
// xxx 	}
// xxx     }
// xxx
// xxx     Si.push_back(i+b);
// xxx     _bt_ab[i+1-b] = (_Node_Base*)(_bgt_a + (i+1));  // L(2n-b) <- 2n
// xxx     _bt_ab[i+b] = _bt_ab[k];                        // L(2n-1+b) <- L(k)
// xxx     _bt_ab[k] = (_Node_Base*)(_bgt_a + i);          // L(k) <- 2n - 1
// xxx   }
// xxx
// xxx   // Connect
// xxx   for (i=1; i<_size; i+=2) {
// xxx     _Node_Base* t = (_Node_Base*) (_bgt_a+i);
// xxx     t->_left = _bt_ab[i];
// xxx     t->_right = _bt_ab[i+1];
// xxx     t->_left->_parent = t->_right->_parent = t;
// xxx   }
// xxx
// xxx   for (i=0; i<_size; i+=2) {
// xxx     _bgt_a[i]._left = _bgt_a[i]._right = 0;
// xxx   }
// xxx
// xxx   _root = _bt_ab[0];
// xxx   _root->_parent = 0;
// xxx   rand_dir((_Node*)_root);
// xxx   assert(is_valid());
// xxx }

// xxx void slicing_flrpln::randomize()
// xxx {
// xxx   assert(_size % 2 || !"size is an odd number");
// xxx   _bt_ab[0] = (_Node_Base*) _bgt_a;     // L(0) <- 0
// xxx   size_t i = 1;                         // n <- 0
// xxx   for (; i<_size; i+=2) {               // terminate the algorithm if n =
// N
// xxx     const size_t X = rand() % (2*i);    // random integer btw 0 and 4n -
// 3
// xxx     const size_t b = X % 2;             // b <- X mod 2
// xxx     const size_t k = X / 2;             // k <- floor(X/2)
// xxx     _bt_ab[i+1-b] = (_Node_Base*)(_bgt_a + (i+1));  // L(2n-b) <- 2n
// xxx     _bt_ab[i+b] = _bt_ab[k];                        // L(2n-1+b) <- L(k)
// xxx     _bt_ab[k] = (_Node_Base*)(_bgt_a + i);          // L(k) <- 2n - 1
// xxx   }
// xxx
// xxx   // Connect
// xxx   for (i=1; i<_size; i+=2) {
// xxx     _Node_Base* t = (_Node_Base*) (_bgt_a+i);
// xxx     t->_left = _bt_ab[i]; // ???
// xxx     t->_right = _bt_ab[i+1]; // ???
// xxx     t->_left->_parent = t;
// xxx     t->_right->_parent = t;
// xxx   }
// xxx
// xxx   for (i=0; i<_size; i+=2) {
// xxx     _bgt_a[i]._left = 0;
// xxx     _bgt_a[i]._right = 0;
// xxx   }
// xxx
// xxx   _root = _bt_ab[0];
// xxx   _root->_parent = 0;
// xxx   rand_dir((_Node*)_root);
// xxx   assert(is_valid());
// xxx }

void slicing_flrpln::check_canonical(const bgt_node *t) const {
  if (t == nullptr)
    return;

  const bgt_node *child = (bgt_node *)t->_left;
  check_canonical(child);
  child = (bgt_node *)t->_right;
  check_canonical(child);
  if (child != nullptr && child->_direct == t->_direct)
    throw non_canonical_flrpln();
}

bool slicing_flrpln::is_canonical() const {
  try {
    check_canonical((bgt_node *)_root);
  } catch (non_canonical_flrpln &) {
    return false;
  }
  return true;
}

// xxx void slicing_flrpln::flip( std::ostream& os, size_t n, size_t& c ) const
// xxx {
// xxx   print(os, c);
// xxx   _bgt_a[n-1]._direct = !_bgt_a[n-1]._direct;
// xxx }
// xxx
// xxx
// xxx void slicing_flrpln::BRGC ( std::ostream& os, size_t n, size_t& c ) const
// xxx {
// xxx   if (n == 0) return;
// xxx
// xxx   BRGC( os, n-1, c );
// xxx   flip( os, n, c );
// xxx   BRGC( os, n-1, c );
// xxx }

std::string slicing_flrpln::SS() const {
  char c = '0';
  std::string ss("");
  SS_recur1((bgt_node *)_root, c);
  SS_recur(ss, (bgt_node *)_root);
  return ss;
}

void slicing_flrpln::SS_recur1(const bgt_node *t, char &c) const {
  if (t->is_leaf()) {
    t->_rank = ++c;
    return;
  }
  auto *a = (bgt_node *)t->_left;
  auto *b = (bgt_node *)t->_right;
  SS_recur1(a, c);
  SS_recur1(b, c);
}

void slicing_flrpln::SS_recur(std::string &ss, const bgt_node *t) const {
  if (t->is_leaf()) {
    ss += t->_rank;
    return;
  }
  auto *a = (bgt_node *)t->_left;
  auto *b = (bgt_node *)t->_right;
  if (t->_direct)
    std::swap(a, b);
  SS_recur(ss, a);
  SS_recur(ss, b);
}

/** Print out the flrpln structure in .dot format to os */
void slicing_flrpln::print_it(std::ostream &os, const bgt_node *t, size_t &d,
                              bool direct) const {
  if (t->is_leaf()) {
    os << d++;
    return;
  }

  if (t->_direct != direct)
    os << "{";
  print_it(os, (bgt_node *)t->_left, d, t->_direct);
  os << "|";
  print_it(os, (bgt_node *)t->_right, d, t->_direct);
  if (t->_direct != direct)
    os << "}";
}

/** Print out the flrpln structure in .dot format to os */
void slicing_flrpln::print(std::ostream &os, size_t &) const {
  // xxx try { check_canonical((bgt_node*) _root); }
  // xxx catch (non_canonical_flrpln&) { return; } // don't print

  size_t d = 1;
  // xxx os << "  s" << rank() << "a" << c++ << " [label=\"";
  os << "a"
     << " [label=\"";
  print_it(os, (bgt_node *)_root, d, false);
  os << "\"];\n";
}

/**
 * Print out all the canonical floorplans in .dot format (given
 * one particular slicing tree structure) according to the
 * Binary Reflected Gray Code order.
 */
std::ostream &operator<<(std::ostream &os, const slicing_flrpln &flrpln) {
  os << "digraph g"
     << " {\n"
     << "  node [shape=record];\n";

  // xxx size_t n = flrpln.num_nodes();
  size_t c = 0; // counter
  // xxx flrpln.BRGC(os, n, c);
  // xxx flrpln.flip(os, n, c);

  flrpln.print(os, c);
  // xxx --c;
  // xxx for (; c>0; --c) {
  // xxx   os << "  s" << flrpln.rank() << "a" << c-1 << " -> "
  // xxx      << "s" << flrpln.rank() << "a" << c << " ;\n";
  // xxx }

  os << "}\n";
  os << std::endl;
  return os;
}
};
