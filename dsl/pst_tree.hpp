// PST tree implementation -*- C++ -*-

/** @file pst_tree.hpp
 *  This is an internal header file, included by other library headers.
 *  You should not attempt to use it directly.
 */

#ifndef _PST_TREE_HPP
#define _PST_TREE_HPP

/*

Priority search tree class, designed for use in implementing interval set
associative containers (intv_set, intv_multiset, intv_map, and int_multimap).
The algorithms are based on those in Edward M. McCreight, "Priority Search
Trees," SIAM J. Comput., Vol. 14, No. 2, pp.257-276, May 1985, and Cormen,
Leiserson, and Rivest, Introduction to Algorithms (MIT Press, 1990),
except that

(1) Only leave nodes contain actual data.
(2)

*/

#include <bits/stl_algobase.h>
//#include <bits/stl_alloc.h>
#include <bits/concept_check.h>
#include <bits/stl_construct.h>
#include <bits/stl_function.h>
#include <cassert>
#include <svgstream.hpp>

class svgstream;

namespace std {
enum _Pst_tree_color { _Pst_M_red = false, _Pst_M_black = true };

/** Base class of _Pst_tree_node */
struct _Pst_tree_node_base {
  typedef _Pst_tree_node_base *_Base_ptr;

  _Pst_tree_color _M_color;
  _Base_ptr _M_parent;
  _Base_ptr _M_left;
  _Base_ptr _M_right;

  static _Base_ptr _S_minimum(_Base_ptr __x) {
    while (__x->_M_left != nullptr)
      __x = __x->_M_left;
    return __x;
  }

  static _Base_ptr _S_maximum(_Base_ptr __x) {
    while (__x->_M_right != nullptr)
      __x = __x->_M_right;
    return __x;
  }
};

/** Node of pst_tree */
template <typename _Val, typename _Key>
struct _Pst_tree_node : public _Pst_tree_node_base {
  typedef _Pst_tree_node<_Val, _Key> *_Link_type;
  _Val _M_value_field;  // x value
  _Key _M_value_field2; // y value
  bool _M_is_winner;    // keep track if it is a winner
};

/** Iterator for pst_tree_base */
struct _Pst_tree_base_iterator {
  typedef _Pst_tree_node_base::_Base_ptr _Base_ptr;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  _Base_ptr _M_node;

  void _M_increment() {
    if (_M_node->_M_right != nullptr) {
      _M_node = _M_node->_M_right;
      while (_M_node->_M_left != nullptr)
        _M_node = _M_node->_M_left;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_right) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      if (_M_node->_M_right != __y)
        _M_node = __y;
    }
  }

  void _M_decrement() {
    if (_M_node->_M_color == _Pst_M_red // why depend on color?
        && _M_node->_M_parent->_M_parent == _M_node)
      _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != nullptr) {
      _Base_ptr __y = _M_node->_M_left;
      while (__y->_M_right != nullptr)
        __y = __y->_M_right;
      _M_node = __y;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_left) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      _M_node = __y;
    }
  }
};

/** Iterator for pst_tree */
template <typename _Val, typename _Ref, typename _Ptr, typename _Key,
          typename _Compare>
struct _Pst_tree_iterator : public _Pst_tree_base_iterator {
  typedef _Val value_type;
  typedef _Ref reference;
  typedef _Ptr pointer;
  typedef _Pst_tree_iterator<_Val, _Val &, _Val *, _Key, _Compare> iterator;
  typedef _Pst_tree_iterator<_Val, const _Val &, const _Val *, _Key, _Compare>
      const_iterator;
  typedef _Pst_tree_iterator<_Val, _Ref, _Ptr, _Key, _Compare> _Self;
  typedef _Pst_tree_node<_Val, _Key> *_Link_type;

  _Compare _M_key_compare; // added by luk
  _Key _M_value2;          // added by luk

  _Pst_tree_iterator() = default;

  _Pst_tree_iterator(_Link_type __x, _Compare __comp) {
    _M_node = __x;
    _M_value2 = 0;
    _M_key_compare = __comp;
  }

  _Pst_tree_iterator(_Link_type __x, _Key __y, _Compare __comp) {
    _M_node = __x;
    _M_value2 = __y;
    _M_key_compare = __comp;
  }

  _Pst_tree_iterator(const iterator &__it) {
    _M_node = __it._M_node;
    _M_value2 = __it._M_value2;
    _M_key_compare = __it._M_key_compare;
  }

  reference operator*() const { return _Link_type(_M_node)->_M_value_field; }

  pointer operator->() const { return &(operator*()); }

  reference x() const { return _Link_type(_M_node)->_M_value_field; }

  // Added by luk
  _Key &y() const { return _Link_type(_M_node)->_M_value_field2; }

  static bool _S_is_leaf(_Pst_tree_node_base::_Base_ptr _node) {
    return _node->_M_left == nullptr && _node->_M_right == nullptr;
  }

  static bool _S_is_header(_Pst_tree_node_base::_Base_ptr _node) {
    return _node->_M_color == _Pst_M_red &&
           _node->_M_parent->_M_parent == _node;
  }

  static _Key &_S_key2(_Pst_tree_node_base::_Base_ptr _node) {
    return _Link_type(_node)->_M_value_field2;
  }

  static bool _S_is_winner(_Pst_tree_node_base::_Base_ptr _node) {
    return _Link_type(_node)->_M_is_winner;
  }

  bool can_move_on(_Pst_tree_node_base::_Base_ptr _node) const {
    return _node != nullptr &&
           (_S_is_winner(_node) || !_M_key_compare(_M_value2, _S_key2(_node)));
  }
  void increment() {
    if (!can_move_on(_M_node)) {
      _M_node = _M_node->_M_parent;
      return;
    }
    if (can_move_on(_M_node->_M_right)) {
      _M_node = _M_node->_M_right;
      while (can_move_on(_M_node->_M_left))
        _M_node = _M_node->_M_left;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_right) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      if (_M_node->_M_right != __y)
        _M_node = __y;
    }
  }

  // Need to be modified
  void decrement() {
    if (_S_is_header(_M_node))
      _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != nullptr) {
      _Base_ptr __y = _M_node->_M_left;
      while (__y->_M_right != nullptr)
        __y = __y->_M_right;
      _M_node = __y;
    } else {
      _Base_ptr __y = _M_node->_M_parent;
      while (_M_node == __y->_M_left) {
        _M_node = __y;
        __y = __y->_M_parent;
      }
      _M_node = __y;
    }
  }

  // Modified by luk
  _Self &operator++() {
    // Only stop at leaf nodes
    do
      increment();
    while (!(_S_is_leaf(_M_node) || _S_is_header(_M_node)));
    return *this;
  }

  _Self operator++(int) {
    _Self __tmp = *this;
    ++*this; // modified by luk
    return __tmp;
  }

  // Modified by luk
  _Self &operator--() {
    // Only stop at leaf nodes
    do
      decrement();
    while (!(_S_is_leaf(_M_node) || _S_is_header(_M_node)));
    return *this;
  }

  _Self operator--(int) {
    _Self __tmp = *this;
    --*this; // modified by luk
    return __tmp;
  }
};

template <typename _Val, typename _Ref, typename _Ptr, typename _Key,
          typename _Compare>
inline bool
operator==(const _Pst_tree_iterator<_Val, _Ref, _Ptr, _Key, _Compare> &__x,
           const _Pst_tree_iterator<_Val, _Ref, _Ptr, _Key, _Compare> &__y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val, typename _Key, typename _Compare>
inline bool operator==(
    const _Pst_tree_iterator<_Val, const _Val &, const _Val *, _Key, _Compare>
        &__x,
    const _Pst_tree_iterator<_Val, _Val &, _Val *, _Key, _Compare> &__y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val, typename _Key, typename _Compare>
inline bool
operator==(const _Pst_tree_iterator<_Val, _Val &, _Val *, _Key, _Compare> &__x,
           const _Pst_tree_iterator<_Val, const _Val &, const _Val *, _Key,
                                    _Compare> &__y) {
  return __x._M_node == __y._M_node;
}

template <typename _Val, typename _Ref, typename _Ptr, typename _Key,
          typename _Compare>
inline bool
operator!=(const _Pst_tree_iterator<_Val, _Ref, _Ptr, _Key, _Compare> &__x,
           const _Pst_tree_iterator<_Val, _Ref, _Ptr, _Key, _Compare> &__y) {
  return __x._M_node != __y._M_node;
}

template <typename _Val, typename _Key, typename _Compare>
inline bool operator!=(
    const _Pst_tree_iterator<_Val, const _Val &, const _Val *, _Key, _Compare>
        &__x,
    const _Pst_tree_iterator<_Val, _Val &, _Val *, _Key, _Compare> &__y) {
  return __x._M_node != __y._M_node;
}

template <typename _Val, typename _Key, typename _Compare>
inline bool
operator!=(const _Pst_tree_iterator<_Val, _Val &, _Val *, _Key, _Compare> &__x,
           const _Pst_tree_iterator<_Val, const _Val &, const _Val *, _Key,
                                    _Compare> &__y) {
  return __x._M_node != __y._M_node;
}

// Changed to non-inline
void _Pst_tree_rotate_left(_Pst_tree_node_base *__x,
                           _Pst_tree_node_base *&__root);
// Changed to non-inline
void _Pst_tree_rotate_right(_Pst_tree_node_base *__x,
                            _Pst_tree_node_base *&__root);

// Changed to non-inline
void _Pst_tree_rebalance(_Pst_tree_node_base *__x,
                         _Pst_tree_node_base *&__root);

_Pst_tree_node_base *
_Pst_tree_rebalance_for_erase(_Pst_tree_node_base *__z,
                              _Pst_tree_node_base *&__root);
// _Pst_tree_node_base*& __leftmost,
// _Pst_tree_node_base*& __rightmost);

// Base class to encapsulate the differences between old SGI-style
// allocators and standard-conforming allocators.  In order to avoid
// having an empty base class, we arbitrarily move one of rb_tree's
// data members into the base class.

/** _Base for general standard-conforming allocators. */
template <typename _Tp, typename _Key, typename _Alloc, bool _S_instanceless>
class _Pst_tree_alloc_base {
public:
  typedef typename _Alloc::template rebind<_Pst_tree_node<_Tp, _Key>>::other
      allocator_type;

  allocator_type get_allocator() const { return _M_node_allocator; }

  _Pst_tree_alloc_base(const allocator_type &__a)
      : _M_node_allocator(__a), _M_header(nullptr) {}

protected:
  allocator_type _M_node_allocator;

  _Pst_tree_node<_Tp, _Key> *_M_header;

  _Pst_tree_node<_Tp, _Key> *_M_get_node() {
    return _M_node_allocator.allocate(1);
  }

  void _M_put_node(_Pst_tree_node<_Tp, _Key> *__p) {
    _M_node_allocator.deallocate(__p, 1);
  }
};

// xxx /** Specialization for instanceless allocators.*/
// xxx template<typename _Tp, typename _Key, typename _Alloc>
// xxx class _Pst_tree_alloc_base<_Tp, _Key, _Alloc, true>
// xxx {
// xxx public:
// xxx   typedef _Alloc allocator_type;
// xxx   allocator_type get_allocator() const { return allocator_type(); }
// xxx
// xxx   _Pst_tree_alloc_base(const allocator_type&) : _M_header(0) {}
// xxx
// xxx protected:
// xxx   _Pst_tree_node<_Tp, _Key>* _M_header;
// xxx
// xxx   typedef _Alloc _Alloc_type;
// xxx
// xxx   _Pst_tree_node<_Tp, _Key>*
// xxx   _M_get_node() { return _Alloc_type::allocate(1); }
// xxx
// xxx   void
// xxx   _M_put_node(_Pst_tree_node<_Tp, _Key>* __p) {
// _Alloc_type::deallocate(__p, 1); }
// xxx };

/** Base class of pst_tree */
template <typename _Tp, typename _Key, typename _Alloc>
struct _Pst_tree_base : public _Pst_tree_alloc_base<_Tp, _Key, _Alloc, true> {
  typedef _Pst_tree_alloc_base<_Tp, _Key, _Alloc, true> _Base;
  typedef typename _Base::allocator_type allocator_type;

  _Pst_tree_base(const allocator_type &__a) : _Base(__a) {
    _Base::_M_header = _Base::_M_get_node();
  }
  ~_Pst_tree_base() { _Base::_M_put_node(_Base::_M_header); }
};

/** Priority Search Tree */
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc = allocator<_Val>>
class _Pst_tree : protected _Pst_tree_base<_Val, _Key, _Alloc> {
  typedef _Pst_tree_base<_Val, _Key, _Alloc> _Base;
  typedef _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> _Self;
  // friend svgstream& operator<< <>(svgstream& os, const _Self& cs);

protected:
  typedef _Pst_tree_node_base *_Base_ptr;
  typedef _Pst_tree_node<_Val, _Key> _pst_tree_node;

public:
  typedef _Key key_type;
  typedef _Val value_type;
  typedef value_type *pointer;
  typedef const value_type *const_pointer;
  typedef value_type &reference;
  typedef const value_type &const_reference;
  typedef _pst_tree_node *_Link_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

  typedef typename _Base::allocator_type allocator_type;
  allocator_type get_allocator() const { return _Base::get_allocator(); }

protected:
  using _Base::_M_get_node;
  using _Base::_M_put_node;
  using _Base::_M_header;

  _Link_type _M_create_node(const value_type &__x, const key_type &__y) {
    _Link_type __tmp = _M_get_node();
    try {
      _Construct(&__tmp->_M_value_field, __x);
      _Construct(&__tmp->_M_value_field2, __y); // added by luk
      __tmp->_M_is_winner = false;              // added by luk
    } catch (...) {
      _M_put_node(__tmp);
      __throw_exception_again;
    }
    return __tmp;
  }

  _Link_type _M_clone_node(_Link_type __x) {
    _Link_type __tmp =
        _M_create_node(__x->_M_value_field, __x->_M_value_field2);
    __tmp->_M_is_winner = __x->_M_is_winner; // added by luk
    __tmp->_M_color = __x->_M_color;
    __tmp->_M_left = nullptr;
    __tmp->_M_right = nullptr;
    return __tmp;
  }

  void destroy_node(_Link_type __p) {
    _Destroy(&__p->_M_value_field);
    _Destroy(&__p->_M_value_field2); // added by luk
    _M_put_node(__p);
  }

  size_type _M_node_count; // keeps track of size of tree
  _Compare _M_key_compare;

  _Link_type &_M_root() const { return (_Link_type &)_M_header->_M_parent; }

  _Link_type _M_leftmost() const {
    if (_M_root() == nullptr)
      return _M_header;
    return _S_minimum(_M_root());
  }

  _Link_type _M_rightmost() const {
    if (_M_root() == 0)
      return _M_header;
    return _S_maximum(_M_root());
  }

  static _Link_type &_S_left(_Link_type __x) {
    return (_Link_type &)(__x->_M_left);
  }

  static _Link_type &_S_right(_Link_type __x) {
    return (_Link_type &)(__x->_M_right);
  }

  static _Link_type &_S_parent(_Link_type __x) {
    return (_Link_type &)(__x->_M_parent);
  }

  static reference _S_value(_Link_type __x) { return __x->_M_value_field; }

  static _Key &_S_value2(_Link_type __x) { return __x->_M_value_field2; }

  static _Key &_S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }

  static _Key &_S_key2(_Link_type __x) { return _S_value2(__x); }

  static _Pst_tree_color &_S_color(_Link_type __x) { return __x->_M_color; }

  static _Link_type &_S_left(_Base_ptr __x) {
    return (_Link_type &)(__x->_M_left);
  }

  static _Link_type &_S_right(_Base_ptr __x) {
    return (_Link_type &)(__x->_M_right);
  }

  static _Link_type &_S_parent(_Base_ptr __x) {
    return (_Link_type &)(__x->_M_parent);
  }

  static reference _S_value(_Base_ptr __x) {
    return ((_Link_type)__x)->_M_value_field;
  }

  static reference _S_value2(_Base_ptr __x) {
    return ((_Link_type)__x)->_M_value_field2;
  }

  static bool _S_is_winner(_Base_ptr __x) {
    return ((_Link_type)__x)->_M_is_winner;
  }

  static const _Key &_S_key(_Base_ptr __x) {
    return _KeyOfValue()(_S_value(_Link_type(__x)));
  }

  static const _Key &_S_key2(_Base_ptr __x) {
    return _S_value2(_Link_type(__x));
  }

  static _Pst_tree_color &_S_color(_Base_ptr __x) {
    return (_Link_type(__x)->_M_color);
  }

  static _Link_type _S_minimum(_Link_type __x) {
    return (_Link_type)_Pst_tree_node_base::_S_minimum(__x);
  }

  static _Link_type _S_maximum(_Link_type __x) {
    return (_Link_type)_Pst_tree_node_base::_S_maximum(__x);
  }

public:
  typedef _Pst_tree_iterator<value_type, reference, pointer, _Key, _Compare>
      iterator;
  typedef _Pst_tree_iterator<value_type, const_reference, const_pointer, _Key,
                             _Compare>
      const_iterator;

  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> rv_iterator;

private:
  iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type &__v,
                     const key_type &__w);

  // Added by luk
  void _M_rebalance(_Pst_tree_node_base *__x, _Pst_tree_node_base *&__root);

  // Added by luk
  _Pst_tree_node_base *_M_rebalance_for_erase(_Pst_tree_node_base *__z,
                                              _Pst_tree_node_base *&__root);
  // _Pst_tree_node_base*& __leftmost,
  // _Pst_tree_node_base*& __rightmost);

  // Added by luk
  void _M_tournament_up(_Link_type __x);

  // Added by luk
  void _M_tournament_up_for_erase(_Link_type __x);

  _Link_type _M_copy(_Link_type __x, _Link_type __p);

  void _M_erase(_Link_type __x);

public:
  // allocation/deallocation
  _Pst_tree() : _Base(allocator_type()), _M_node_count(0), _M_key_compare() {
    _M_empty_initialize();
  }

  _Pst_tree(const _Compare &__comp)
      : _Base(allocator_type()), _M_node_count(0), _M_key_compare(__comp) {
    _M_empty_initialize();
  }

  _Pst_tree(const _Compare &__comp, const allocator_type &__a)
      : _Base(__a), _M_node_count(0), _M_key_compare(__comp) {
    _M_empty_initialize();
  }

  _Pst_tree(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x)
      : _Base(__x.get_allocator()), _M_node_count(0),
        _M_key_compare(__x._M_key_compare) {
    if (__x._M_root() == 0)
      _M_empty_initialize();
    else {
      _S_color(_M_header) = _Pst_M_red;
      _M_root() = _M_copy(__x._M_root(), _M_header);
      // xxx _M_leftmost() = _S_minimum(_M_root());
      // xxx _M_rightmost() = _S_maximum(_M_root());
    }
    _M_node_count = __x._M_node_count;
  }

  ~_Pst_tree() { clear(); }

  _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &
  operator=(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x);

  // Added by luk
  void _M_rotate_left(_Link_type __x, _Pst_tree_node_base *&__root) {
    _Link_type __y = _S_right(__x);
    _Link_type __lx = _S_left(__x);
    _Link_type __ly = _S_left(__y);
    _Link_type __ry = _S_right(__y);

    assert(__lx != 0);
    assert(__ly != 0);
    assert(__ry != 0);

    if (!__y->_M_is_winner) {
      assert(__lx->_M_is_winner);
      assert(_S_value2(__lx) == _S_value2(__x));
      _S_value2(__y) = _S_value2(__x);
      __y->_M_is_winner = __x->_M_is_winner;
      __x->_M_is_winner = true;
      __ly->_M_is_winner = false;
      __ry->_M_is_winner = false;
    } else { // y is a winner
      assert(!__lx->_M_is_winner);
      __y->_M_is_winner = __x->_M_is_winner;
      if (!__ly->_M_is_winner) {
        __x->_M_is_winner = false;
        // replay
        bool __comp = _M_key_compare(_S_key2(__lx), _S_key2(__ly));
        if (__comp) {
          __lx->_M_is_winner = true;
          __ly->_M_is_winner = false;
          _S_value2(__x) = _S_value2(__lx);
        } else {
          __ly->_M_is_winner = true;
          __lx->_M_is_winner = false;
          _S_value2(__x) = _S_value2(__ly);
        }
      } else {
        assert(__ry != 0 && !__ry->_M_is_winner);
        __x->_M_is_winner = true;
      }
    }
    _Pst_tree_rotate_left(__x, __root);
  }

  // Added by luk. Same as _M_rotate_left with "right" <-> "left"
  void _M_rotate_right(_Link_type __x, _Pst_tree_node_base *&__root) {
    _Link_type __y = _S_left(__x);
    _Link_type __rx = _S_right(__x);
    _Link_type __ry = _S_right(__y);
    _Link_type __ly = _S_left(__y);

    assert(__rx != 0);
    assert(__ry != 0);
    assert(__ly != 0);

    if (!__y->_M_is_winner) {
      assert(__rx->_M_is_winner);
      assert(_S_value2(__rx) == _S_value2(__x));
      _S_value2(__y) = _S_value2(__x);
      __y->_M_is_winner = __x->_M_is_winner;
      __x->_M_is_winner = true;
      __ry->_M_is_winner = false;
      __ly->_M_is_winner = false;
    } else { // y is a winner
      assert(!__rx->_M_is_winner);
      __y->_M_is_winner = __x->_M_is_winner;
      if (!__ry->_M_is_winner) {
        __x->_M_is_winner = false;
        // replay
        bool __comp = _M_key_compare(_S_key2(__rx), _S_key2(__ry));
        if (__comp) {
          __rx->_M_is_winner = true;
          __ry->_M_is_winner = false;
          _S_value2(__x) = _S_value2(__rx);
        } else {
          __ry->_M_is_winner = true;
          __rx->_M_is_winner = false;
          _S_value2(__x) = _S_value2(__ry);
        }
      } else {
        assert(!__ly->_M_is_winner);
        __x->_M_is_winner = true;
      }
    }
    _Pst_tree_rotate_right(__x, __root);
  }

private:
  void _M_empty_initialize() {
    _S_color(_M_header) = _Pst_M_red; // used to distinguish header from
    // __root, in iterator.operator++
    _M_root() = nullptr;
    _M_header->_M_left = _M_header;
    _M_header->_M_right = _M_header;
  }

public:
  // Accessors.
  _Compare key_comp() const { return _M_key_compare; }

  iterator begin() { return iterator(_M_leftmost(), key_comp()); }

  const_iterator begin() const {
    return const_iterator(_M_leftmost(), key_comp());
  }

  iterator end() { return iterator(_M_header, key_comp()); }

  const_iterator end() const { return const_iterator(_M_header, key_comp()); }

  rv_iterator rbegin() { return rv_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }

  rv_iterator rend() { return rv_iterator(begin()); }

  const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  bool empty() const { return _M_node_count == 0; }

  size_type size() const { return _M_node_count; }

  size_type max_size() const { return size_type(-1); }

  void swap(_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__t) {
    std::swap(_M_header, __t._M_header);
    std::swap(_M_node_count, __t._M_node_count);
    std::swap(_M_key_compare, __t._M_key_compare);
  }

  // Insert/erase.
  pair<iterator, bool> insert_unique(const value_type &__x,
                                     const key_type &__y);

  iterator insert_equal(const value_type &__x, const key_type &__y);

  iterator insert_unique(iterator __position, const value_type &__x,
                         const key_type &__y);

  iterator insert_equal(iterator __position, const value_type &__x,
                        const key_type &__y);

  template <typename _InputIterator, typename _InputIterator2>
  void insert_unique(_InputIterator __first, _InputIterator __last,
                     _InputIterator2 __first2);

  template <typename _InputIterator, typename _InputIterator2>
  void insert_equal(_InputIterator __first, _InputIterator __last,
                    _InputIterator2 __first2);

  void erase(iterator __position);

  size_type erase(const key_type &__x, const key_type &__y);

  void erase(iterator __first, iterator __last);

  void erase(const key_type *__first, const key_type *__last,
             const key_type *__first2);

  void clear() {
    if (_M_node_count != 0) {
      _M_erase(_M_root());
      // xxx _M_leftmost() = _M_header;
      _M_root() = nullptr;
      // xxx _M_rightmost() = _M_header;
      _M_node_count = 0;
    }
  }

  // Set operations.
  iterator find(const key_type &__x, const key_type &__y);

  const_iterator find(const key_type &__x, const key_type &__y) const;

  size_type count(const key_type &__x, const key_type &__y) const;

  iterator lower_bound(const key_type &__x, const key_type &__y);

  const_iterator lower_bound(const key_type &__x, const key_type &__y) const;

  _Link_type lower_bound_recur(_Link_type __n, const key_type &__x,
                               const key_type &__y) const;

  iterator upper_bound(const key_type &__x, const key_type &__y);

  const_iterator upper_bound(const key_type &__x, const key_type &__y) const;

  _Link_type upper_bound_recur(_Link_type __n, const key_type &__x,
                               const key_type &__y) const;

  pair<iterator, iterator> equal_range(const key_type &__x,
                                       const key_type &__y);

  pair<const_iterator, const_iterator> equal_range(const key_type &__x,
                                                   const key_type &__y) const;

  // Debugging.
  bool __pst_verify() const;
  bool __pst_verify2() const;
  bool __pst_verify_heap(_Link_type __x) const;
};

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator==(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return __x.size() == __y.size() && equal(__x.begin(), __x.end(), __y.begin());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator<(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
          const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return lexicographical_compare(__x.begin(), __x.end(), __y.begin(),
                                 __y.end());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator!=(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return !(__x == __y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator>(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
          const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return __y < __x;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator<=(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return !(__y < __x);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline bool
operator>=(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  return !(__x < __y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline void swap(_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x,
                 _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__y) {
  __x.swap(__y);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
operator=(const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &__x) {
  if (this != &__x) {
    // Note that _Key may be a constant type.
    clear();
    _M_node_count = 0;
    _M_key_compare = __x._M_key_compare;
    if (__x._M_root() == 0) {
      _M_root() = 0;
      // _M_leftmost() = _M_header;
      // _M_rightmost() = _M_header;
    } else {
      _M_root() = _M_copy(__x._M_root(), _M_header);
      // _M_leftmost() = _S_minimum(_M_root());
      // _M_rightmost() = _S_maximum(_M_root());
      _M_node_count = __x._M_node_count;
    }
  }
  return *this;
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_insert(
    _Base_ptr __x_, _Base_ptr __y_, const _Val &__v, const _Key &__w) {
  auto __x = (_Link_type)__x_;
  auto __y = (_Link_type)__y_;
  _Link_type __z;
  _Link_type __z2; // insert two nodes

  if (__y == _M_header) { // empty tree
    __z = _M_create_node(__v, __w);
    _S_left(__y) = __z;
    _M_root() = __z;
    // _M_rightmost() = __z;
    _S_parent(__z) = __y;
  } else {
    assert(_S_left(__y) == 0);
    assert(_S_right(__y) == 0);

    __z = _M_create_node(__v, __w);
    __z2 = _M_clone_node(__y);
    __z2->_M_parent = __y->_M_parent;
    __y->_M_color = __z->_M_color;

    if (__x != nullptr || _M_key_compare(_KeyOfValue()(__v), _S_key(__y))) {
      _S_left(__z2) = __z; // also makes _M_leftmost() = __z
      _S_right(__z2) = __y;
      // __y could be leftmost and rightmost at the same time (i.e y=root)
      // if (__y == _M_leftmost())
      //_M_leftmost() = __z; // maintain _M_leftmost() pointing to min node
    } else {
      _S_right(__z2) = __z;
      _S_left(__z2) = __y;
      _S_key(__z2) = _KeyOfValue()(__v);
      // __y could be leftmost and rightmost at the same time (i.e y=root)
      // if (__y == _M_rightmost())
      // _M_rightmost() = __z;

      // Maintain _M_rightmost() pointing to max node.
    }

    if (__y == _M_root())
      _M_root() = __z2;
    else {
      if (_S_left(__y->_M_parent) == __y)
        _S_left(__y->_M_parent) = __z2;
      else
        _S_right(__y->_M_parent) = __z2;
    }
    _S_parent(__z) = __z2;
    _S_parent(__y) = __z2;
  }

  _S_left(__z) = nullptr;
  _S_right(__z) = nullptr;

  _M_tournament_up(__z); // added by luk
  assert(__pst_verify2());

  /// xxx if (__y != _M_header)
  /// xxx   _M_rebalance(__y, _M_header->_M_parent);
  __y->_M_color = _Pst_M_red;
  _M_rebalance(__z, _M_header->_M_parent);
  ++_M_node_count;
  assert(__pst_verify());
  return iterator(__z, key_comp());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
_Pst_tree_node_base *
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_rebalance_for_erase(
    _Pst_tree_node_base *__z, _Pst_tree_node_base *&__root) {
  assert(__z->_M_left == 0 || __z->_M_right == 0 || !"only delete leave nodes");

  _Pst_tree_node_base *__y = __z;
  _Pst_tree_node_base *__x = nullptr;
  _Pst_tree_node_base *__x_parent = nullptr;

  if (__y->_M_left == nullptr) // __z has at most one non-null child. y == z.
    __x = __y->_M_right;       // __x might be null.
  else
    __x = __y->_M_left; // __x is not null.
  __x_parent = __y->_M_parent;
  if (__x)
    __x->_M_parent = __y->_M_parent;
  if (__root == __z)
    __root = __x;
  else if (__z->_M_parent->_M_left == __z)
    __z->_M_parent->_M_left = __x;
  else
    __z->_M_parent->_M_right = __x;
  // if (__leftmost == __z)
  //   if (__z->_M_right == 0)        // __z->_M_left must be null also
  //     __leftmost = __z->_M_parent;
  // // makes __leftmost == _M_header if __z == __root
  //   else
  //     __leftmost = _Pst_tree_node_base::_S_minimum(__x);
  // if (__rightmost == __z)
  //   if (__z->_M_left == 0)         // __z->_M_right must be null also
  //     __rightmost = __z->_M_parent;
  // // makes __rightmost == _M_header if __z == __root
  //   else                      // __x == __z->_M_left
  //     __rightmost = _Pst_tree_node_base::_S_maximum(__x);
  // xxx       }
  if (__y->_M_color != _Pst_M_red) {
    while (__x != __root && (__x == nullptr || __x->_M_color == _Pst_M_black))
      if (__x == __x_parent->_M_left) {
        _Pst_tree_node_base *__w = __x_parent->_M_right;
        if (__w->_M_color == _Pst_M_red) {
          __w->_M_color = _Pst_M_black;
          __x_parent->_M_color = _Pst_M_red;
          _M_rotate_left((_Link_type)__x_parent, __root);
          // xxx assert(__pst_verify_heap(__x));
          __w = __x_parent->_M_right;
        }
        if ((__w->_M_left == nullptr ||
             __w->_M_left->_M_color == _Pst_M_black) &&
            (__w->_M_right == nullptr ||
             __w->_M_right->_M_color == _Pst_M_black)) {
          __w->_M_color = _Pst_M_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_right == nullptr ||
              __w->_M_right->_M_color == _Pst_M_black) {
            if (__w->_M_left)
              __w->_M_left->_M_color = _Pst_M_black;
            __w->_M_color = _Pst_M_red;
            _M_rotate_right((_Link_type)__w, __root);
            __w = __x_parent->_M_right;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _Pst_M_black;
          if (__w->_M_right)
            __w->_M_right->_M_color = _Pst_M_black;
          _M_rotate_left((_Link_type)__x_parent, __root);
          break;
        }
      } else {
        // Same as above, with "right" <-> "left".
        _Pst_tree_node_base *__w = __x_parent->_M_left;
        if (__w->_M_color == _Pst_M_red) {
          __w->_M_color = _Pst_M_black;
          __x_parent->_M_color = _Pst_M_red;
          _M_rotate_right((_Link_type)__x_parent, __root);
          __w = __x_parent->_M_left;
        }
        if ((__w->_M_right == nullptr ||
             __w->_M_right->_M_color == _Pst_M_black) &&
            (__w->_M_left == nullptr ||
             __w->_M_left->_M_color == _Pst_M_black)) {
          __w->_M_color = _Pst_M_red;
          __x = __x_parent;
          __x_parent = __x_parent->_M_parent;
        } else {
          if (__w->_M_left == nullptr ||
              __w->_M_left->_M_color == _Pst_M_black) {
            if (__w->_M_right)
              __w->_M_right->_M_color = _Pst_M_black;
            __w->_M_color = _Pst_M_red;
            _M_rotate_left((_Link_type)__w, __root);
            __w = __x_parent->_M_left;
          }
          __w->_M_color = __x_parent->_M_color;
          __x_parent->_M_color = _Pst_M_black;
          if (__w->_M_left)
            __w->_M_left->_M_color = _Pst_M_black;
          _M_rotate_right((_Link_type)__x_parent, __root);
          // xxx assert(__pst_verify_heap(__x));
          break;
        }
      }
    if (__x)
      __x->_M_color = _Pst_M_black;
  }
  return __y;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_rebalance(
    _Pst_tree_node_base *__x, _Pst_tree_node_base *&__root) {
  __x->_M_color = _Pst_M_red;
  while (__x != __root && __x->_M_parent->_M_color == _Pst_M_red) {
    if (__x->_M_parent == __x->_M_parent->_M_parent->_M_left) {
      _Pst_tree_node_base *__y = __x->_M_parent->_M_parent->_M_right;
      if (__y && __y->_M_color == _Pst_M_red) {
        __x->_M_parent->_M_color = _Pst_M_black;
        __y->_M_color = _Pst_M_black;
        __x->_M_parent->_M_parent->_M_color = _Pst_M_red;
        __x = __x->_M_parent->_M_parent;
      } else {
        if (__x == __x->_M_parent->_M_right) {
          __x = __x->_M_parent;
          _M_rotate_left((_Link_type)__x, __root);
        }
        __x->_M_parent->_M_color = _Pst_M_black;
        __x->_M_parent->_M_parent->_M_color = _Pst_M_red;
        _M_rotate_right((_Link_type)__x->_M_parent->_M_parent, __root);
      }
    } else {
      _Pst_tree_node_base *__y = __x->_M_parent->_M_parent->_M_left;
      if (__y && __y->_M_color == _Pst_M_red) {
        __x->_M_parent->_M_color = _Pst_M_black;
        __y->_M_color = _Pst_M_black;
        __x->_M_parent->_M_parent->_M_color = _Pst_M_red;
        __x = __x->_M_parent->_M_parent;
      } else {
        if (__x == __x->_M_parent->_M_left) {
          __x = __x->_M_parent;
          _M_rotate_right((_Link_type)__x, __root);
        }
        __x->_M_parent->_M_color = _Pst_M_black;
        __x->_M_parent->_M_parent->_M_color = _Pst_M_red;
        _M_rotate_left((_Link_type)__x->_M_parent->_M_parent, __root);
      }
    }
  }
  __root->_M_color = _Pst_M_black;
}

// Added by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_tournament_up(
    _Link_type __x) {
  while (__x != _M_root()) {
    _Link_type __p = _S_parent(__x);
    _Link_type __y = (__x == _S_left(__p)) ? _S_right(__p) : _S_left(__p);
    assert(__y != 0);
    bool __comp = _M_key_compare(_S_key2(__x), _S_key2(__y));
    if (!__comp) {
      __y->_M_is_winner = true;
      assert(!__x->_M_is_winner);
      return;
    }
    __x->_M_is_winner = true;
    __y->_M_is_winner = false;
    _S_key2(__p) = _S_key2(__x);

    while (__p->_M_is_winner) {
      assert(__p != _M_root() && "root cannot be a winner");
      __x = __p;
      __p = _S_parent(__p);
      _S_key2(__p) = _S_key2(__x);
    }
    __x = __p;
  }
}

// Added by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare,
               _Alloc>::_M_tournament_up_for_erase(_Link_type __x) {
  while (__x->_M_is_winner) {
    assert(__x != _M_root());
    // replay
    _Link_type __p = _S_parent(__x);
    _Link_type __y = (__x == _S_left(__p)) ? _S_right(__p) : _S_left(__p);
    bool __comp = _M_key_compare(_S_key2(__x), _S_key2(__y));
    if (__comp) {
      __x->_M_is_winner = true;
      __y->_M_is_winner = false;
      _S_value2(__p) = _S_value2(__x);
    } else {
      __y->_M_is_winner = true;
      __x->_M_is_winner = false;
      _S_value2(__p) = _S_value2(__y);
    }
    __x = __p;
  }
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_equal(
    const _Val &__v, const _Key &__w) {
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  while (__x != nullptr) {
    __y = __x;
    __x = _M_key_compare(_KeyOfValue()(__v), _S_key(__x)) ? _S_left(__x)
                                                          : _S_right(__x);
  }
  return _M_insert(__x, __y, __v, __w);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
pair<typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator,
     bool>
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_unique(
    const _Val &__v, const _Key &__w) {
  _Link_type __y = _M_header;
  _Link_type __x = _M_root();
  bool __comp = true;
  while (__x != nullptr) {
    __y = __x;
    __comp = _M_key_compare(_KeyOfValue()(__v), _S_key(__x));
    __x = __comp ? _S_left(__x) : _S_right(__x);
  }
  iterator __j = iterator(__y, __w, key_comp());
  if (__comp) {
    if (__j == begin())
      return pair<iterator, bool>(_M_insert(__x, __y, __v, __w), true);
    else
      --__j;
  }
  if (_M_key_compare(_S_key(__j._M_node), _KeyOfValue()(__v)))
    return pair<iterator, bool>(_M_insert(__x, __y, __v, __w), true);
  return pair<iterator, bool>(__j, false);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_unique(
    iterator __position, const _Val &__v, const _Key &__w) {
  if (__position._M_node == _M_header->_M_left) {
    // begin()
    if (size() > 0 &&
        _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node)))
      return _M_insert(__position._M_node, __position._M_node, __v, __w);
    // first argument just needs to be non-null
    else
      return insert_unique(__v, __w).first;
  } else if (__position._M_node == _M_header) {
    // end()
    if (_M_key_compare(_S_key(_M_rightmost()), _KeyOfValue()(__v)))
      return _M_insert(0, _M_rightmost(), __v, __w);
    else
      return insert_unique(__v, __w).first;
  } else {
    iterator __before = __position;
    --__before;
    if (_M_key_compare(_S_key(__before._M_node), _KeyOfValue()(__v)) &&
        _M_key_compare(_KeyOfValue()(__v), _S_key(__position._M_node))) {
      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v, __w);
      else
        return _M_insert(__position._M_node, __position._M_node, __v, __w);
      // first argument just needs to be non-null
    } else
      return insert_unique(__v, __w).first;
  }
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::insert_equal(
    iterator __position, const _Val &__v, const _Key &__w) {
  if (__position._M_node == _M_header->_M_left) {
    // begin()
    if (size() > 0 &&
        !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v)))
      return _M_insert(__position._M_node, __position._M_node, __v, __w);
    // first argument just needs to be non-null
    else
      return insert_equal(__v, __w);
  } else if (__position._M_node == _M_header) {
    // end()
    if (!_M_key_compare(_KeyOfValue()(__v), _S_key(_M_rightmost())))
      return _M_insert(0, _M_rightmost(), __v, __w);
    else
      return insert_equal(__v, __w);
  } else {
    iterator __before = __position;
    --__before;
    if (!_M_key_compare(_KeyOfValue()(__v), _S_key(__before._M_node)) &&
        !_M_key_compare(_S_key(__position._M_node), _KeyOfValue()(__v))) {
      if (_S_right(__before._M_node) == 0)
        return _M_insert(0, __before._M_node, __v, __w);
      else
        return _M_insert(__position._M_node, __position._M_node, __v, __w);
      // first argument just needs to be non-null
    } else
      return insert_equal(__v, __w);
  }
}

template <typename _Key, typename _Val, typename _KoV, typename _Cmp,
          typename _Alloc>
template <class _II, class _II2>
void _Pst_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_equal(_II __first,
                                                             _II __last,
                                                             _II2 __first2) {
  for (; __first != __last; ++__first, ++__first2)
    insert_equal(*__first, *__first2);
}

template <typename _Key, typename _Val, typename _KoV, typename _Cmp,
          typename _Alloc>
template <class _II, class _II2>
void _Pst_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::insert_unique(_II __first,
                                                              _II __last,
                                                              _II2 __first2) {
  for (; __first != __last; ++__first, ++__first2)
    insert_unique(*__first, *__first2);
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    iterator __position) {
  // Added by luk
  auto __x = (_Link_type)__position._M_node;
  assert(__x->_M_left == 0 && __x->_M_right == 0);

  if (__x == _M_header->_M_parent) { // x is the root
    clear();
    return;
  }

  _Link_type __p = _S_parent(__x);
  // y is the sibling of x
  _Link_type __y = (__x == _S_left(__p)) ? _S_right(__p) : _S_left(__p);
  assert(__y != 0);

  if (__p == _M_root()) { // ???
    __y->_M_parent = __p->_M_parent;
    __y->_M_is_winner = __p->_M_is_winner;
    __y->_M_color = __p->_M_color;
    _M_root() = __y;
    // _M_leftmost() = __y;
    // _M_rightmost() = __y;
    // _M_rebalance(__y, _M_header->_M_parent); // ???
    destroy_node(__x);
    destroy_node(__p);
    --_M_node_count;
    assert(__pst_verify());
    return;
  }

  if (__x->_M_is_winner) {
    _S_value2(__p) = _S_value2(__y);
    _M_tournament_up_for_erase(__p);
  }

  if (__x == _S_left(__p))
    _S_left(__p) = nullptr;
  else
    _S_right(__p) = nullptr;

  __y->_M_is_winner = __p->_M_is_winner;

  _M_rebalance_for_erase(__p, _M_header->_M_parent);

  destroy_node(__x);
  destroy_node(__p);

  --_M_node_count;
  assert(__pst_verify2());
  assert(__pst_verify());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(const _Key &__x,
                                                            const _Key &__y) {
  pair<iterator, iterator> __p = equal_range(__x, __y);
  size_type __n = distance(__p.first, __p.second);
  erase(__p.first, __p.second);
  return __n;
}

template <typename _Key, typename _Val, typename _KoV, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type
_Pst_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_M_copy(_Link_type __x,
                                                       _Link_type __p) {
  // Structural copy.  __x and __p must be non-null.
  _Link_type __top = _M_clone_node(__x);
  __top->_M_parent = __p;

  try {
    if (__x->_M_right)
      __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);

    while (__x != 0) {
      _Link_type __y = _M_clone_node(__x);
      __p->_M_left = __y;
      __y->_M_parent = __p;
      if (__x->_M_right)
        __y->_M_right = _M_copy(_S_right(__x), __y);
      __p = __y;
      __x = _S_left(__x);
    }
  } catch (...) {
    _M_erase(__top);
    __throw_exception_again;
  }
  return __top;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_M_erase(
    _Link_type __x) {
  // Erase without rebalancing.
  while (__x != nullptr) {
    _M_erase(_S_right(__x));
    _Link_type __y = _S_left(__x);
    destroy_node(__x);
    __x = __y;
  }
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    iterator __first, iterator __last) {
  if (__first == begin() && __last == end())
    clear();
  else
    while (__first != __last)
      erase(__first++);
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
void _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::erase(
    const _Key *__first, const _Key *__last, const _Key *__first2) {
  while (__first != __last)
    erase(*__first++, *__first2++);
}

// Todo: need to be modified
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::find(const _Key &__k,
                                                           const _Key &__l) {
  _Link_type __y = _M_header; // Last node which is not less than __k.
  _Link_type __x = _M_root(); // Current node.

  while (__x != 0)
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);

  iterator __j = iterator(__y, __l, key_comp());
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                    : __j;
}

// Todo: need to be modified
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::find(
    const _Key &__k, const _Key &__l) const {
  _Link_type __y = _M_header; // Last node which is not less than __k.
  _Link_type __x = _M_root(); // Current node.

  while (__x != 0) {
    if (!_M_key_compare(_S_key(__x), __k))
      __y = __x, __x = _S_left(__x);
    else
      __x = _S_right(__x);
  }
  const_iterator __j = const_iterator(__y, __l, key_comp());
  return (__j == end() || _M_key_compare(__k, _S_key(__j._M_node))) ? end()
                                                                    : __j;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::count(
    const _Key &__k, const _Key &__l) const {
  pair<const_iterator, const_iterator> __p = equal_range(__k, __l);
  size_type __n = distance(__p.first, __p.second);
  return __n;
}

// Added by luk: Mixed iterative and recursive function
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::lower_bound_recur(
    _Link_type __x, const _Key &__k, const _Key &__l) const {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  while (__x) {
    if (!_S_is_winner(__x) && _M_key_compare(__l, _S_key2(__x)))
      return __y; // not found
    if (_S_left(__x) == nullptr && _S_right(__x) == nullptr) {
      if (!_M_key_compare(_S_key(__x), __k))
        return __x; // found
      return __y;   // not found
    }
    if (!_M_key_compare(_S_key(__x), __k))
      __y = lower_bound_recur(_S_left(__x), __k, __l);
    if (__y != _M_header)
      return __y;
    __x = _S_right(__x);
  }
  return __y;
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::lower_bound(
    const _Key &__k, const _Key &__l) {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  _Link_type __x = _M_root(); /* Current node. */
  __y = lower_bound_recur(__x, __k, __l);
  return iterator(__y, __l, key_comp());
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::lower_bound(
    const _Key &__k, const _Key &__l) const {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  _Link_type __x = _M_root(); /* Current node. */
  __y = lower_bound_recur(__x, __k, __l);
  return const_iterator(__y, __l, key_comp());
}

// Added by luk: Mixed iterative and recursive function
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::_Link_type
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::upper_bound_recur(
    _Link_type __x, const _Key &__k, const _Key &__l) const {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  while (__x) {
    if (!_S_is_winner(__x) && _M_key_compare(__l, _S_key2(__x)))
      return __y; // not found
    if (_S_left(__x) == nullptr && _S_right(__x) == nullptr) {
      if (_M_key_compare(__k, _S_key(__x)))
        return __x; // found
      return __y;   // not found
    }
    if (_M_key_compare(__k, _S_key(__x)))
      __y = upper_bound_recur(_S_left(__x), __k, __l);
    if (__y != _M_header)
      return __y;
    __x = _S_right(__x);
  }
  return __y;
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::upper_bound(
    const _Key &__k, const _Key &__l) {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  _Link_type __x = _M_root(); /* Current node. */
  __y = upper_bound_recur(__x, __k, __l);
  return iterator(__y, __l, key_comp());
}

// Modified by luk
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::const_iterator
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::upper_bound(
    const _Key &__k, const _Key &__l) const {
  _Link_type __y = _M_header; /* Last node which is not less than __k. */
  _Link_type __x = _M_root(); /* Current node. */
  __y = upper_bound_recur(__x, __k, __l);
  return const_iterator(__y, __l, key_comp());
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
inline pair<
    typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator,
    typename _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator>
_Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::equal_range(
    const _Key &__k, const _Key &__l) {
  return pair<iterator, iterator>(lower_bound(__k, __l), upper_bound(__k, __l));
}

template <typename _Key, typename _Val, typename _KoV, typename _Compare,
          typename _Alloc>
inline pair<
    typename _Pst_tree<_Key, _Val, _KoV, _Compare, _Alloc>::const_iterator,
    typename _Pst_tree<_Key, _Val, _KoV, _Compare, _Alloc>::const_iterator>
_Pst_tree<_Key, _Val, _KoV, _Compare, _Alloc>::equal_range(
    const _Key &__k, const _Key &__l) const {
  return pair<const_iterator, const_iterator>(lower_bound(__k, __l),
                                              upper_bound(__k, __l));
}

int __black_count(_Pst_tree_node_base *__node, _Pst_tree_node_base *__root);

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
bool _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::__pst_verify()
    const {
  if (_M_node_count == 0 || begin() == end())
    return _M_node_count == 0 && begin() == end() &&
           _M_header->_M_left == _M_header && _M_header->_M_right == _M_header;

  int __len = __black_count(_M_leftmost(), _M_root());
  for (const_iterator __it = begin(); __it != end(); __it._M_increment()) {
    auto __x = (_Link_type)__it._M_node;
    _Link_type __L = _S_left(__x);
    _Link_type __R = _S_right(__x);

    if (__x->_M_color == _Pst_M_red)
      if ((__L && __L->_M_color == _Pst_M_red) ||
          (__R && __R->_M_color == _Pst_M_red))
        return false;
    if ((__L == 0) != (__R == 0))
      return false;
    if (__L && _M_key_compare(_S_key(__x), _S_key(__L)))
      return false;
    if (__R && _M_key_compare(_S_key(__R), _S_key(__x)))
      return false;
    if (!__L && !__R && __black_count(__x, _M_root()) != __len)
      return false;

    // Added by luk
    if (__L && _M_key_compare(_S_key2(__L), _S_key2(__x)))
      return false;
    if (__R && _M_key_compare(_S_key2(__R), _S_key2(__x)))
      return false;

    if (__L && __R) {
      if (_M_key_compare(_S_key2(__R), _S_key2(__L))) {
        if (__L->_M_is_winner)
          return false;
        if (!__R->_M_is_winner)
          return false;
        if (_S_key2(__R) != _S_key2(__x))
          return false;
      } else if (_M_key_compare(_S_key2(__L), _S_key2(__R))) {
        if (__R->_M_is_winner)
          return false;
        if (!__L->_M_is_winner)
          return false;
        if (_S_key2(__L) != _S_key2(__x))
          return false;
      } else {
        if (__R->_M_is_winner && __L->_M_is_winner)
          return false;
        if (!__R->_M_is_winner && !__L->_M_is_winner)
          return false;
      }
    }
  }

  // if (_M_leftmost() != _Pst_tree_node_base::_S_minimum(_M_root()))
  //   return false;
  // if (_M_rightmost() != _Pst_tree_node_base::_S_maximum(_M_root()))
  //   return false;
  return true;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
bool _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::__pst_verify_heap(
    _Link_type __x) const {
  // xxx _Link_type __x = (_Link_type) __it._M_node;
  _Link_type __L = _S_left(__x);
  _Link_type __R = _S_right(__x);

  // Added by luk
  if (__L && _M_key_compare(_S_key2(__L), _S_key2(__x)))
    return false;
  if (__R && _M_key_compare(_S_key2(__R), _S_key2(__x)))
    return false;

  if (__L && __R) {
    if (_M_key_compare(_S_key2(__R), _S_key2(__L))) {
      if (__L->_M_is_winner)
        return false;
      if (_S_key2(__R) != _S_key2(__x))
        return false;
    }
    if (_M_key_compare(_S_key2(__L), _S_key2(__R))) {
      if (__R->_M_is_winner)
        return false;
      if (_S_key2(__L) != _S_key2(__x))
        return false;
    }
  }
  return true;
}

template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
bool _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::__pst_verify2()
    const {
  for (const_iterator __it = begin(); __it != end(); __it._M_increment()) {
    auto __x = (_Link_type)__it._M_node;
    if (!__pst_verify_heap(__x))
      return false;
  }

  return true;
}

/** Print the point in SVG format.*/
template <typename _Key, typename _Val>
void _Print_recur(svgstream &osvgs, _Key bl, _Key bu,
                  const _Pst_tree_node<_Val, _Key> *pst_node);

/** Print the point in SVG format.*/
template <typename _Key, typename _Val, typename _KeyOfValue, typename _Compare,
          typename _Alloc>
svgstream &
operator<<(svgstream &osvgs,
           const _Pst_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc> &pst) {
  _Print_recur(osvgs, -10000, 10000, pst._M_root());
  return osvgs;
}

template <typename _Key, typename _Val>
void _Print_recur(svgstream &osvgs, _Key bl, _Key bu,
                  const _Pst_tree_node<_Val, _Key> *pst_node) {
  if (!pst_node)
    return;

  auto x = (_Key)pst_node->_M_value_field;
  auto y = (_Key)pst_node->_M_value_field2;

  if (!pst_node->_M_left && !pst_node->_M_right) {
    osvgs << "<circle"
          << " cx=\"" << x << "\""
          << " cy=\"" << y << "\""
          << " r=\"8\""
          << " style=\"fill:red;stroke:black\""
          << "/>";
  } else {
    osvgs << "<line"
          << " x1=\"" << bl << "\""
          << " y1=\"" << y << "\""
          << " x2=\"" << bu << "\""
          << " y2=\"" << y << "\""
          << " style=\"fill:green;stroke:green\""
          << "/>\n";
    osvgs << "<line"
          << " x1=\"" << x << "\""
          << " y1=\"" << y << "\""
          << " x2=\"" << x << "\""
          << " y2=\"" << 10000 << "\""
          << " style=\"fill:green;stroke:green\""
          << "/>\n";
  }

  typedef _Pst_tree_node<_Val, _Key> *_Link;
  _Print_recur(osvgs, bl, x, (_Link)pst_node->_M_left);
  _Print_recur(osvgs, x, bu, (_Link)pst_node->_M_right);
}

} // namespace std

#endif
