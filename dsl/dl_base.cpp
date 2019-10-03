#include "dl_base.hpp"
#include "dsl_algo.hpp"
#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <cassert>
#include <cfloat> // for DBL_MAX
#include <iostream>
#include <vector>

using namespace boost::lambda;
using std::vector;

namespace dsl {

/** Create a doubly linked list. */
dl_base::dl_base(size_t n) : _size(n), _sab(new node *[n]) { assert(n > 0); }

/** Destructor */
dl_base::~dl_base() { delete[] _sab; }

/** @return the cost between two nodes. Default is a unit cost. */
double dl_base::cost(const dnode_base *, const dnode_base *) const {
  return 1.0;
}

/** Print out the structure in .dot format to os */
std::ostream &dl_base::print_clist(std::ostream &os,
                                   const dnode_base *tail) const {
  size_t i;

  assert(tail->loop_back());

  os << "digraph g {\n"
     << "  node [shape=circle]\n";
  for (i = 0; i < num_nodes(); ++i) {
    os << "  n" << _sab[i] << " [label=\"" << i + 1 << "\"];\n";
  }

  os << "  tail [shape=plaintext];\n";
  os << "  tail -> n" << tail << ";\n";

  dnode_base::const_iterator c1(tail->_next);
  dnode_base::const_iterator c2(c1);

  do {
    os << "  n" << (c1++).value();
    os << " -> n" << c1.value() << ";\n";
  } while (c1 != c2);

  return os;
}

// Explicit instantiations
template class dlist_base<int>;
}
