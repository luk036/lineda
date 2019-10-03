// Sample header file to illustrate the coding policy. The rules are
// based on the books "Large-Scale C++ Software Design" and "Effective
// C++, 2nd Ed"
//
//  DEF - definition
//  MDR - major design rule
//  mdr - minor design rule
//  GL  - guideline
//  P   - principle
//
//  effc++ - rules from Effective C++.
//

#include "snode.hpp"
#include <boost/static_assert.hpp>

namespace dsl {

BOOST_STATIC_ASSERT(sizeof(snode_base) <= 4);

snode_base *snode_base::josephus_permutate(int m) {
  int i;

  assert(m > 0);
  assert(loop_back());

  if (m == 1)
    return this;

  snode_base *tail = this;
  for (i = 0; i < m - 1; ++i)
    tail = tail->_next;
  snode_base *const head = tail->_next;
  tail->_next = head->_next;

  snode_base *cn = head; // effc++: Postpone variable definitions
                         // as long as possible (Item 32)
  while (cn != tail) {
    for (i = 0; i < m - 1; ++i)
      tail = tail->_next;
    cn = cn->_next = tail->_next;
    tail->_next = cn->_next;
  }

  tail = cn->_next;
  tail->_next = head;

  assert(tail->loop_back());
  return tail;
}
};
