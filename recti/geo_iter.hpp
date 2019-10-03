#ifndef RECTI_GEO_ITERATOR
#define RECTI_GEO_ITERATOR

namespace recti {
template <class Iterator>

/** Iterator with x() and y() member functions */
class geo_iterator {
  typedef geo_iterator<Iterator> _Self;

public:
  typedef typename Iterator::value_type::value_type value_type;

  /** Create an iterator from base class */
  geo_iterator(Iterator &it) : _it(it) {}

  /** @return the x value */
  value_type x() const { return _it->x(); }

  /** @return the y value */
  value_type y() const { return _it->y(); }

  /** @return the next iterator */
  _Self next() const {
    Iterator it = _it.next();
    return _Self(it);
  }

  /** Prefix increment: advance one node */
  _Self &operator++() {
    ++_it;
    return *this;
  }

  /** Postfix increment: advance one node */
  _Self operator++(int) {
    _Self old(*this);
    ++*this;
    return old;
  }

  /** Prefix decrement: advance backward one node */
  _Self &operator--() {
    --_it;
    return *this;
  }

  /** Postfix decrement: advance backward one node */
  _Self operator--(int) {
    _Self old(*this);
    --*this;
    return old;
  }

  /** Equal */
  bool operator==(const _Self &x) const { return _it == x._it; }

  /** Not equal */
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  Iterator _it;
};

/** Dual iterator of geo_iterator, i.e. x() and y() are interchange */
template <class Iterator> class geo_dual_iterator {
  typedef geo_dual_iterator<Iterator> _Self;

public:
  typedef typename Iterator::value_type::value_type value_type;

  /** Create an iterator from base class */
  geo_dual_iterator(Iterator &it) : _it(it) {}

  value_type x() const { return _it->y(); }

  value_type y() const { return _it->x(); }

  _Self next() const {
    Iterator it = _it.next();
    return _Self(it);
  }

  /** Prefix increment: advance one node */
  _Self &operator++() {
    ++_it;
    return *this;
  }

  /** Postfix increment: advance one node */
  _Self operator++(int) {
    _Self old(*this);
    ++*this;
    return old;
  }

  /** Prefix decrement: advance backward one node */
  _Self &operator--() {
    --_it;
    return *this;
  }

  /** Postfix decrement: advance backward one node */
  _Self operator--(int) {
    _Self old(*this);
    --*this;
    return old;
  }

  /** Equal */
  bool operator==(const _Self &x) const { return _it == x._it; }

  /** Not equal */
  bool operator!=(const _Self &x) const { return _it != x._it; }

private:
  Iterator _it;
};
}

#endif
