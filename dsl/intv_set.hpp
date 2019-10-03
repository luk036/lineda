// Set implementation -*- C++ -*-

// Copyright (C) 2001 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/** @file intv_set.hpp
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef __INTV_SET_HPP
#define __INTV_SET_HPP

#include <bits/concept_check.h>
#include "pst_tree.hpp"
#include <functional>

namespace std {

// Forward declarations of operators < and ==, needed for friend declaration.

template <class _Key, class _Compare = less<_Key>,
          class _Alloc = allocator<_Key>>
class intv_set;

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const intv_set<_Key, _Compare, _Alloc> &__x,
                       const intv_set<_Key, _Compare, _Alloc> &__y);

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const intv_set<_Key, _Compare, _Alloc> &__x,
                      const intv_set<_Key, _Compare, _Alloc> &__y);

/** interval set */
template <class _Key, class _Compare, class _Alloc> class intv_set {
  // concept requirements
  //__glibcpp_class_requires(_Key, _SGIAssignableConcept)
  //__glibcpp_class_requires4(_Compare, bool, _Key, _Key,
  //_BinaryFunctionConcept);

public:
  // typedefs:
  typedef _Key key_type;
  typedef _Key value_type;
  typedef _Compare key_compare;
  typedef _Compare value_compare;
  typedef intv_set<_Key, _Compare, _Alloc> _Self;
  // friend svgstream& operator<< <>(svgstream& os, const _Self& cs);

private:
  typedef _Pst_tree<key_type, value_type, _Identity<value_type>, key_compare,
                    _Alloc>
      _Rep_type;
  _Rep_type _M_t; // red-black tree representing intv_set
public:
  // typedef typename _Rep_type::const_pointer pointer;
  // typedef typename _Rep_type::const_pointer const_pointer;
  typedef typename _Rep_type::const_reference reference;
  typedef typename _Rep_type::const_reference const_reference;
  typedef typename _Rep_type::const_iterator iterator;
  typedef typename _Rep_type::const_iterator const_iterator;
  typedef typename _Rep_type::const_reverse_iterator reverse_iterator;
  typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename _Rep_type::size_type size_type;
  typedef typename _Rep_type::difference_type difference_type;
  typedef typename _Rep_type::allocator_type allocator_type;

  // allocation/deallocation

  intv_set() : _M_t(_Compare(), allocator_type()) {}
  explicit intv_set(const _Compare &__comp,
                    const allocator_type &__a = allocator_type())
      : _M_t(__comp, __a) {}

  template <class _InputIterator>
  intv_set(_InputIterator __first, _InputIterator __last,
           _InputIterator __first2)
      : _M_t(_Compare(), allocator_type()) {
    _M_t.insert_unique(__first, __last, __first2);
  }

  template <class _InputIterator>
  intv_set(_InputIterator __first, _InputIterator __last,
           _InputIterator __first2, const _Compare &__comp,
           const allocator_type &__a = allocator_type())
      : _M_t(__comp, __a) {
    _M_t.insert_unique(__first, __last, __first2);
  }

  intv_set(const intv_set<_Key, _Compare, _Alloc> &__x) : _M_t(__x._M_t) {}
  intv_set<_Key, _Compare, _Alloc> &
  operator=(const intv_set<_Key, _Compare, _Alloc> &__x) {
    _M_t = __x._M_t;
    return *this;
  }

  // accessors:

  key_compare key_comp() const { return _M_t.key_comp(); }
  value_compare value_comp() const { return _M_t.key_comp(); }
  allocator_type get_allocator() const { return _M_t.get_allocator(); }

  iterator begin() const { return _M_t.begin(); }
  iterator end() const { return _M_t.end(); }
  reverse_iterator rbegin() const { return _M_t.rbegin(); }
  reverse_iterator rend() const { return _M_t.rend(); }
  bool empty() const { return _M_t.empty(); }
  size_type size() const { return _M_t.size(); }
  size_type max_size() const { return _M_t.max_size(); }
  void swap(intv_set<_Key, _Compare, _Alloc> &__x) { _M_t.swap(__x._M_t); }

  // insert/erase
  pair<iterator, bool> insert(const value_type &__x, const key_type &__y) {
    pair<typename _Rep_type::iterator, bool> __p = _M_t.insert_unique(__x, __y);
    return pair<iterator, bool>(__p.first, __p.second);
  }
  iterator insert(iterator __position, const value_type &__x,
                  const key_type &__y) {
    typedef typename _Rep_type::iterator _Rep_iterator;
    return _M_t.insert_unique((_Rep_iterator &)__position, __x, __y);
  }
  template <class _InputIterator>
  void insert(_InputIterator __first, _InputIterator __last,
              _InputIterator __first2) {
    _M_t.insert_unique(__first, __last, __first2);
  }
  void erase(iterator __position) {
    typedef typename _Rep_type::iterator _Rep_iterator;
    _M_t.erase((_Rep_iterator &)__position);
  }
  size_type erase(const key_type &__x, const key_type &__y) {
    return _M_t.erase(__x, __y);
  }
  void erase(iterator __first, iterator __last) {
    typedef typename _Rep_type::iterator _Rep_iterator;
    _M_t.erase((_Rep_iterator &)__first, (_Rep_iterator &)__last);
  }
  void clear() { _M_t.clear(); }

  // intv_set operations:

  size_type count(const key_type &__x, const key_type &__y) const {
    return _M_t.find(__x, __y) == _M_t.end() ? 0 : 1;
  }

#ifdef _GLIBCPP_RESOLVE_LIB_DEFECTS
  // 214.  intv_set::find() missing const overload
  iterator find(const key_type &__x, const key_type &__y) {
    return _M_t.find(__x, __y);
  }
  const_iterator find(const key_type &__x, const key_type &__y) const {
    return _M_t.find(__x, __y);
  }
  iterator lower_bound(const key_type &__x, const key_type &__y) {
    return _M_t.lower_bound(__x, __y);
  }
  const_iterator lower_bound(const key_type &__x, const key_type &__y) const {
    return _M_t.lower_bound(__x, __y);
  }
  iterator upper_bound(const key_type &__x, const key_type &__y) {
    return _M_t.upper_bound(__x, __y);
  }
  const_iterator upper_bound(const key_type &__x, const key_type &__y) const {
    return _M_t.upper_bound(__x, __y);
  }
  pair<iterator, iterator> equal_range(const key_type &__x,
                                       const key_type &__y) {
    return _M_t.equal_range(__x, __y);
  }
  pair<const_iterator, const_iterator> equal_range(const key_type &__x,
                                                   const key_type &__y) const {
    return _M_t.equal_range(__x, __y);
  }
#else
  iterator find(const key_type &__x, const key_type &__y) const {
    return _M_t.find(__x, __y);
  }
  iterator lower_bound(const key_type &__x, const key_type &__y) const {
    return _M_t.lower_bound(__x, __y);
  }
  iterator upper_bound(const key_type &__x, const key_type &__y) const {
    return _M_t.upper_bound(__x, __y);
  }
  pair<iterator, iterator> equal_range(const key_type &__x,
                                       const key_type &__y) const {
    return _M_t.equal_range(__x, __y);
  }
#endif

  template <class _K1, class _C1, class _A1>
  friend bool operator==(const intv_set<_K1, _C1, _A1> &,
                         const intv_set<_K1, _C1, _A1> &);
  template <class _K1, class _C1, class _A1>
  friend bool operator<(const intv_set<_K1, _C1, _A1> &,
                        const intv_set<_K1, _C1, _A1> &);
};

template <class _Key, class _Compare, class _Alloc>
inline bool operator==(const intv_set<_Key, _Compare, _Alloc> &__x,
                       const intv_set<_Key, _Compare, _Alloc> &__y) {
  return __x._M_t == __y._M_t;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<(const intv_set<_Key, _Compare, _Alloc> &__x,
                      const intv_set<_Key, _Compare, _Alloc> &__y) {
  return __x._M_t < __y._M_t;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator!=(const intv_set<_Key, _Compare, _Alloc> &__x,
                       const intv_set<_Key, _Compare, _Alloc> &__y) {
  return !(__x == __y);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>(const intv_set<_Key, _Compare, _Alloc> &__x,
                      const intv_set<_Key, _Compare, _Alloc> &__y) {
  return __y < __x;
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator<=(const intv_set<_Key, _Compare, _Alloc> &__x,
                       const intv_set<_Key, _Compare, _Alloc> &__y) {
  return !(__y < __x);
}

template <class _Key, class _Compare, class _Alloc>
inline bool operator>=(const intv_set<_Key, _Compare, _Alloc> &__x,
                       const intv_set<_Key, _Compare, _Alloc> &__y) {
  return !(__x < __y);
}

template <class _Key, class _Compare, class _Alloc>
inline void swap(intv_set<_Key, _Compare, _Alloc> &__x,
                 intv_set<_Key, _Compare, _Alloc> &__y) {
  __x.swap(__y);
}

template <class _Key, class _Compare, class _Alloc>
svgstream &operator<<(svgstream &os,
                      const intv_set<_Key, _Compare, _Alloc> &cs) {
  return os << cs._M_t;
}

} // namespace std

#endif /* __GLIBCPP_INTERNAL_INTV_SET_H */

// Local Variables:
// mode:C++
// End: