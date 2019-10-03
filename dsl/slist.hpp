#ifndef DSL_SLIST_HPP
#define DSL_SLIST_HPP

#ifndef DSL_SL_BASE_HPP
#include "sl_base.hpp"
#endif
#include <cassert>

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Singly linked list. */
template <typename _Tp> class slist : public slist_base<_Tp> {
  typedef slist_base<_Tp> _Base;

public:
  /** Create a singly linked list with size n. */
  inline slist(size_t n) : slist_base<_Tp>(n), _head(slist_base<_Tp>::_sa) {
    assert(n > 0);
    slist_base<_Tp>::make_list();
    (slist_base<_Tp>::_sa + n - 1)->_next = nullptr;
  }

  /** Copy constructor */
  inline slist(const slist<_Tp> &copy)
      : slist_base<_Tp>(copy),
        _head(copy._head - copy._sa + slist_base<_Tp>::_sa) {}

  /** Equality. */
  inline bool operator==(const slist<_Tp> &rhs) const {
    if (_head - slist_base<_Tp>::_sa != rhs._head - rhs._sa)
      return false;
    return slist_base<_Tp>::operator==(rhs);
  }

  /** @return whether this is a valid list */
  inline bool is_valid() const { return !_head->has_cycle(); }

  /** Reverse. */
  inline void reverse() { _head = _head->reverse(); }

private:
  // Unimplement
  slist<_Tp> &operator=(const slist<_Tp> &) = delete;

private:
  snode<_Tp> *_head; /**< point to the head of the list */
};

/** @} */ // end of dsl
}

#endif
