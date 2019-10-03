#ifndef G_BPQUEUE_HPP
#define G_BPQUEUE_HPP

#ifndef G_LIST_HPP
#include "glist.hpp"
#endif

namespace dsl {
/**
 * @addtogroup dsl
 * @{
 */

/** Bounded priority queue node */
class bpq_node : public dnode_base {
public:
  typedef dnode_base_iterator<bpq_node> iterator;
  typedef dnode_base_iterator<const bpq_node> const_iterator;

  size_t _key; /**< key */
};

/* Forward declaration */
template <class _Node> class gbpqueueIterator;
template <class _Node> class gbpqueueConstIterator;

/**
 * Bounded Priority Queue with integer keys in [a..b].
 * Implemented by array (bucket) of doubly-linked lists.
 * Efficient if key is bounded by a small integer value.
 * Note that this class does not own the PQ nodes, even though PQ node
 * class is defined in this header file. This feature makes the nodes
 * sharable between doubly linked list class and this class. In the FM
 * algorithm, the node either attached to the gain buckets (PQ) or in the
 * freeVertexList (doubly linked list), but not in both of them in the
 * same time.
 *
 * Another improvement is to make the array size one element bigger
 * i.e. (b - a + 2). The extra dummy array element (which is T[0])
 * is used to reduce the boundary checking during updating.
 *
 * All the member functions assume that the keys are within the bound.
 */
template <class _Node> class gbpqueue {
  friend class gbpqueueIterator<_Node>;
  friend class gbpqueueConstIterator<_Node>;

public:
  gbpqueue() : _offset(0), _high(0), _max(0), _T(0) {}

  /**
   * @brief  Constructor.
   * @param  a   lower bound
   * @param  b   upper bound
   * Precondition: a <= b
   */
  gbpqueue(int a, int b);

  /**
   * Destructor.
   */
  ~gbpqueue() {
    if (_T != 0)
      delete[] _T;
  }

  /**
   * @brief  Setup.
   * @param  a   lower bound
   * @param  b   upper bound
   * Precondition: a <= b
   */
  void setup(int a, int b);

  /**
   * @brief  Get the key.
   * @param  it  the item
   * @return the key of it.
   */
  int get_key(const _Node &it) const { return it._key + _offset; }

  /** @return the max value. */
  int get_max() const { return _max + _offset; }

  /** @return true if the PQ is empty. */
  bool empty() const { return _max == 0; }

  /** @return true if the PQ is valid. Take O(n) time. */
  bool is_valid() const;

  /** (For restricted used only) Make dummy. */
  void make_dummy() { _max = 1; }

  /** Reset the PQ. */
  void clear();

  /**
   * @brief  Insert item at the end of this queue.
   * @param  it  the item
   * @param  k   the key
   *  Precondition: PQ does not contain it; a <= k <= b.
   */
  void push_back(_Node &it, int k);

  /**
   * @brief  Insert item at the end without update the max value.
   * @param  it  the item
   * @param  k   the key
   * Precondition: PQ does not contain it; a <= k <= b.
   * Call resync() after calling this method.
   */
  void push_back_fast(_Node &it, int k);

  /**
   * @brief  Arbitrarily push item without update the max value.
   * @param  it  the item
   * @param  k   the key
   * Precondition: PQ does not contain it; a <= k <= b.
   * Call resync() after calling this method.
   */
  // void pushRandom_fast(_Node& it, int k);

  /** Update the max value */
  void resync();

  /** Pop an item with highest key. Precondition: PQ is not empty.*/
  void pop_front();

  /** @brief  Decrease key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will not be preserved.
      For FM algorithm, this is a prefered behavior. */
  void decrease_key_lifo_by(_Node &it, size_t m);

  /** @brief  Increase key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will not be preserved.
      For FM algorithm, this is a prefered behavior. */
  void increase_key_lifo_by(_Node &it, size_t m);

  void promote(_Node &it);

  void demote(_Node &it);

  /** @brief  Decrease key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will be preserved.
      For FM algorithm, this is a prefered behavior. */
  void decrease_key_by(_Node &it, size_t m);

  /** @brief  Increase key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will be preserved.
      For FM algorithm, this is a prefered behavior. */
  void increase_key_by(_Node &it, size_t m);

  /** @brief  Modify key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will not be preserved.
      For FM algorithm, this is a prefered behavior. */
  void modify_key_lifo_by(_Node &it, int m);

  /** @brief  Modify key by m. Precondition: PQ contains it.
      @param  it  the item
      @param  m   the change of key
      Note that order of items with same key will be preserved.
      For FM algorithm, this is a prefered behavior. */
  void modify_key_by(_Node &it, int m);

  /** Detach the item from this queue. Precondition: PQ contains it */
  void detach(_Node &it);

  /** Push all the items into a given list. This BPQueue becomes empty
      after this operation. */
  void push_to(glist<_Node> &list);

private:
  // Unimplemented
  gbpqueue(const gbpqueue<_Node> &);
  gbpqueue<_Node> &operator=(const gbpqueue<_Node> &);

private:
  int _offset;      /**< a - 1 */
  size_t _high;     /**< b - a + 1 */
  size_t _max;      /**< max value */
  glist<_Node> *_T; /**< bucket, array of lists */
};

/** @} */ // end of group2

template <class _Node>
inline gbpqueue<_Node>::gbpqueue(int a, int b)
    : _offset(a - 1), _high(b - _offset), _max(0),
      _T(new glist<_Node>[_high + 1]) {
  assert(b >= a);
  _T[0].make_dummy(); // sentinel

  /* The propose of_T[0] is to reduce the boundary check. _T[0] acts
     like a non-empty list so that _T[0].isEmpty() always returns
     false. Don't insert items to _T[0]. */
}

template <class _Node> inline void gbpqueue<_Node>::setup(int a, int b) {
  assert(b >= a);

  _offset = a - 1;
  _high = b - _offset;
  _max = 0;
  _T = new glist<_Node>[_high + 1];
  _T[0].make_dummy(); // sentinel

  /* The propose of_T[0] is to reduce the boundary check. _T[0] acts
     like a non-empty list so that _T[0].isEmpty() always returns
     false. Don't insert items to _T[0]. */
}

template <class _Node> inline bool gbpqueue<_Node>::is_valid() const {
  size_t cur = _max;
  while (cur > 0)
    if (!_T[cur--].is_valid())
      return false;
  return true;
}

template <class _Node> inline void gbpqueue<_Node>::clear() {
  while (_max > 0)
    _T[_max--].clear();
}

template <class _Node>
inline void gbpqueue<_Node>::push_back(_Node &it, int k) {
  const size_t key = k - _offset;
  assert(!(key < 1 || key > _high));
  if (_max < key)
    _max = key;
  it._key = key;
  _T[key].push_back(it);
}

template <class _Node>
inline void gbpqueue<_Node>::push_back_fast(_Node &it, int k) {
  const size_t key = k - _offset;
  assert(!(key < 1 || key > _high));
  it._key = key;
  _T[key].push_back(it);
}

// template <class _Node>
// inline void gbpqueue<_Node>::pushRandom_fast(_Node& it, int k)
// {
//   const size_t key = k - _offset;
//   assert(!(key<1 || key>_high));
//   it._key = key;
//   _T[key].pushRandom(it);
// }

template <class _Node> inline void gbpqueue<_Node>::resync() {
  _max = _high;
  while (_T[_max].empty())
    _max--;
}

template <class _Node> inline void gbpqueue<_Node>::pop_front() {
  assert(_max > 0);
  _T[_max].pop_front();
  while (_T[_max].empty())
    _max--;

  /* Without the sentinel, the above while loop needs to check the
     boundary condition, i.e., max > 0:
     while (_max > 0 && _T[_max].empty()) _max--;
     Other functions below may also need the similar check.
  */
}

template <class _Node>
void gbpqueue<_Node>::decrease_key_lifo_by(_Node &it, size_t m) {
  assert(it._key > m);
  glist<_Node>::detach(it);
  it._key -= m;
  _T[it._key].push_front(it);
  while (_T[_max].empty())
    _max--;
}

template <class _Node>
void gbpqueue<_Node>::increase_key_lifo_by(_Node &it, size_t m) {
  glist<_Node>::detach(it);
  size_t &key = it._key;
  key += m;
  assert(key <= _high);
  _T[key].push_front(it);
  if (key > _max)
    _max = key;
}

template <class _Node> void gbpqueue<_Node>::promote(_Node &it) {
  glist<_Node>::detach(it);
  assert(it._key <= _high);
  _T[it._key].push_front(it);
}

template <class _Node> void gbpqueue<_Node>::demote(_Node &it) {
  glist<_Node>::detach(it);
  assert(it._key <= _high);
  _T[it._key].push_back(it);
}

template <class _Node>
void gbpqueue<_Node>::decrease_key_by(_Node &it, size_t m) {
  assert(it._key > m);
  glist<_Node>::detach(it);
  size_t &key = it._key;
  key -= m;
  _T[key].push_back(it);
  while (_T[_max].empty())
    _max--;
}

template <class _Node>
void gbpqueue<_Node>::increase_key_by(_Node &it, size_t m) {
  glist<_Node>::detach(it);
  size_t &key = it._key;
  key += m;
  assert(key <= _high);
  _T[key].push_back(it);
  if (key > _max)
    _max = key;
}

template <class _Node>
inline void gbpqueue<_Node>::modify_key_lifo_by(_Node &it, int m) {
  // Experiments showed that "increase key" is more often than "decrease key"
  if (m > 0)
    increase_key_lifo_by(it, m);
  else if (m < 0)
    decrease_key_lifo_by(it, -m);
}

template <class _Node>
inline void gbpqueue<_Node>::modify_key_by(_Node &it, int m) {
  // Experiments showed that "increase key" is more often than "decrease key"
  if (m > 0)
    increase_key_by(it, m);
  else if (m < 0)
    decrease_key_by(it, -m);
}

template <class _Node> inline void gbpqueue<_Node>::detach(_Node &it) {
  glist<_Node>::detach(it);
  while (_T[_max].empty())
    _max--;
}

template <class _Node>
inline void gbpqueue<_Node>::push_to(glist<_Node> &list) {
  while (_max > 0) {
    if (!_T[_max].empty()) {
      list.concat(_T[_max]);
    }
    --_max;
  }
  assert(empty());
}

/** @addtogroup group2
 *  @{
 */

/**
 * Bounded Priority Queue Iterator. Traverse the queue in descending
 * order. Detaching queue items may invalidate the iterator because
 * the iterator makes a copy of current key.
 */
template <class _Node> class gbpqueueIterator {
  typedef typename glist<_Node>::iterator list_iterator;

private:
  gbpqueue<_Node> *_bpq; /**< the priority queue */
  glist<_Node> *_T;      /**< bucket, array of lists */
  size_t _cur_key;       /**< the current key value */
  list_iterator _lc;     /**< list iterator pointed to
                              the next item. */

private:
  /** @return the key of the current node (unused currently). */
  size_t get_key() const { return _cur_key + _bpq->_offset; }

public:
  /** Default Constructor */
  gbpqueueIterator() {}

  /** Constructor. */
  gbpqueueIterator(gbpqueue<_Node> &BPQ)
      : _bpq(&BPQ), _T(BPQ._T), _cur_key(BPQ._max), _lc(_T[_cur_key]) {}

  /** Associate to the queue BPQ and reset the iterator. */
  void associate(gbpqueue<_Node> &BPQ) {
    _bpq = &BPQ;
    _T = BPQ._T;
    reset();
  }

  /** Reset the iterator to the first position. */
  void reset() {
    _cur_key = _bpq->_max;
    _lc.associate(_T[_cur_key]);
  }

  /** @return true if there is a next item. */
  bool hasNext() const { return _cur_key > 0; }

  /** @return the key of the current node (without offset!).
      For comparsion purpose only. */
  size_t getRel_key() const { return _cur_key; }

  /** @return the next item. Precondition: next item exists. */
  _Node &getNext() {
    assert(hasNext());
    _Node &res = _lc.getNext();
    while (!_lc.hasNext())
      _lc.associate(_T[--_cur_key]);
    return res;
  }
};

/** @} */ // end of dsl
}

#endif
