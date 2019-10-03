#include "snode.hpp"
#include <boost/static_assert.hpp>

namespace dsl {

// Don't increase the size of snode_base as it is used
// as a base in many classes.
BOOST_STATIC_ASSERT(sizeof(snode_base) <= sizeof(void *));

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
snode_base *snode_base::josephus_permutate(int m) {
  int i;

  assert(m > 0);
  assert(loop_back());

  if (m == 1)
    return this;

  snode_base *tail = this;
  for (i = 0; i < m - 1; ++i)
    tail = tail->_next;                 /* move m-1 steps */
  snode_base *const head = tail->_next; /* attach "cn" to the new list */
  tail->_next = head->_next;            /* detach "head" from the old list */

  snode_base *cn = head;
  while (cn != tail) {
    for (i = 0; i < m - 1; ++i)
      tail = tail->_next;         /* move m-1 steps */
    cn = cn->_next = tail->_next; /* attach "cn" to the new list */
    tail->_next = cn->_next;      /* detach "cn" from the old list */
  }

  tail = cn->_next;
  tail->_next = head; /* make circular */

  assert(tail->loop_back());
  return tail;
}

// Explicit instantiations
template class snode_base_iterator<snode_base>;
template class snode<int>;
template class snode_iterator<int, int &, int *>;
};
