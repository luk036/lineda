#include "dnode.hpp"
#include <boost/static_assert.hpp>

namespace dsl {

// Don't increase the size of dnode_base as it is used
// as a base in many classes.
BOOST_STATIC_ASSERT(sizeof(dnode_base) <= 2 * sizeof(void *));

/** Perform (n,m)-Josephus permutation. The Josephus problem is
    defined as follows. Suppose that n people are arranged in a
    circle and that we are given a positive integer m <=
    n. Beginning with a designated first person, we proceed around
    the circle, removing every mth person. After each person is
    removed, counting continues around the circle that remains. This
    process continues until all n people have been removed. The
    order in which the people are removed form the circle defines
    the (n,m)-Josephus permutation of the integers 1, 2, ... n. For
    example, the (7, 3)-Josephus permutation is < 3, 6, 2, 7, 5, 1,
    4 >.
    @return pointer to the tail of the permutated clist.
    Pre-conditions: m > 0 and m <= n.
    Post-condition: this is a valid singly circular linked list.
    Time complexity: O(n*m), Space complexity: O(1) */
dnode_base *dnode_base::josephus_permutate(int m) {
  auto *tail = reinterpret_cast<_Self *>(
      reinterpret_cast<snode_base *>(this)->josephus_permutate(m));

  // Reconstruct the backward pointer (_prev)
  _Self *s = tail;
  do {
    s->_next->_prev = s;
    s = s->_next;
  } while (s != tail);

  assert(tail->loop_back());
  return tail;
}

// Explicit instantiations
template class dnode_base_iterator<dnode_base>;
template class dnode<int>;
template class dnode_iterator<int, int &, int *>;
};
