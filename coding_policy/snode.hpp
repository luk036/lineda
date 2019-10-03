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

#ifndef DSL_SNODE_HPP
#define DSL_SNODE_HPP

#include <cassert>

namespace dsl {

// Def: A component is the smallest unit of physical design. (p.101)
// P: A component is the appropriate fundamental unit of design. (p.103)

// P: Defining an iterator class along with a container class in the
// same component enables user extensibility, improves
// maintainability, and enhances reusability while preserving
// encapsulation.
// P: Design patterns are an effective way of communicating reusable
// concepts and ideas at an architectural level.
template <class _Ref, class _Ptr = _Ref> class snode_base_iterator {
  typedef snode_base_iterator<_Ref, _Ptr> _Self;

public:
  snode_base_iterator() : _p(0) {}
  snode_base_iterator(_Ptr *x) : _p(x) {}

  _Ptr *value() const { return _p; }

  // GL: The semantics of an overloaded operator should be natural,
  // obvious, and intuitive to clients.
  // GL: The syntactic properties of overloaded operators for
  // user-defined types should mirror the properties already defined
  // for the fundamental types.
  // P: Readability should be the primary reason for employing
  // operator overloading.
  _Ref &operator*() const { return *_p; }
  _Ptr *operator->() const { return _p; }
  _Self &operator++() {
    _p = _p->_next;
    return *this;
  }
  _Self operator++(int) {
    const _Self t = *this;
    _p = _p->_next;
    return t;
  }
  bool operator==(const _Self &x) const { return _p == x._p; }
  bool operator!=(const _Self &x) const { return _p != x._p; }
  bool operator==(const _Ptr *p) const { return _p == p; }
  bool operator!=(const _Ptr *p) const { return _p != p; }

private:
  _Ptr *_p;
};

// effc++: Strive for class interfaces that are complete and minimal
// (Item 18).
class snode_base {
  typedef snode_base _Self;
  typedef snode_base_iterator<_Self> iterator;
  typedef snode_base_iterator<const _Self> const_iterator;

  // P: Friendship within a component is an implementation detail of
  // that component. (p.137)
  // P: Granting (local friendship to classes defined with the same
  // component does *not* violate encapsulation. (p.139)
  // P: Friendship affects access privilege but not implied
  // dependency. (p.141)
  friend class snode_base_iterator<_Self>;       // fine
  friend class snode_base_iterator<const _Self>; // fine

private:
  _Self *_next;

public:
  snode_base() : _next(0) {}

  inline void attach_after(_Self *b) {
    b->_next = _next;
    _next = b;
  }
  inline void detach_after() {
    assert(_next != 0);
    _next = _next->_next;
  }
  inline _Self *reverse();
  inline bool cycle_on_path() const;
  inline bool loop_back() const;
  _Self *josephus_permutate(int m);
};

inline snode_base *snode_base::reverse() {
  assert(!cycle_on_path());
  snode_base *res = 0;
  snode_base *head = this;
  while (head != 0) {
    // effc++: Use "const" whenever possible (Item 21)
    snode_base *const tmp = head->_next;
    head->_next = res;
    res = head;
    head = tmp;
  }
  return res;
}

inline bool snode_base::cycle_on_path() const {
  const_iterator n1(this), n2(this);
  while (n2 != 0) {
    if (++n2 == 0)
      return false;
    if (++n2 == ++n1)
      return true;
  }
  return false;
}

inline bool snode_base::loop_back() const {
  const_iterator n1(this), n2(this);
  while (n2 != 0) {
    if (++n2 == this)
      return true;
    else if (n2 == 0)
      return false;
    if (++n2 == this)
      return true;
    else if (n2 == ++n1)
      return false;
  }
  return false;
}

template <typename _Tp> class snode;

// GL: Be consistent about names used in the same way; in particular
// adopt consistent method names and operators for recurring design
// patterns such as iteration.
template <typename _Tp, typename _Ref = _Tp, typename _Ptr = _Tp>
class snode_iterator {
  typedef snode_iterator<_Tp, _Ref, _Ptr> _Self;
  typedef snode_base_iterator<snode<_Tp> > _Base;

public:
  snode_iterator(snode<_Tp> *x) : _it(x) {}
  snode_iterator() : _it(0) {}

  _Ref &operator*() const { return _it->_data; }
  _Ptr *operator->() const { return &_it->_data; }
  _Self &operator++() {
    ++_it;
    return *this;
  }
  _Self operator++(int) {
    const _Self t = *this;
    ++_it;
    return t;
  }
  bool operator==(const _Self &x) const { return _it == x._it; }
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  _Base _it;
};

template <typename _Tp> class snode {
  typedef snode<_Tp> _Self;
  typedef snode_iterator<_Self> iterator;
  typedef snode_iterator<const _Self> const_iterator;

  friend class snode_iterator;

private:
  _Self *_next;
  _Tp _data;

  snode() : _next(0), _data() {}
};
}

#endif
