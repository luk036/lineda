#ifndef NUMERIC_TRIPLE_HPP
#define NUMERIC_TRIPLE_HPP

#include "interval.hpp"
#include <boost/concept_check.hpp>
#include <cassert>
#include <iosfwd>
#include <utility>

namespace numeric {

/**
 * @addtogroup numeric
 * @{
 */

/** A class to represent a triple value {min, typical, max}.
 *  Precondtion: _Tp is lessThanComparable.
 */
template <typename _Tp> class triple {
  BOOST_CLASS_REQUIRE(_Tp, boost, LessThanComparableConcept);
  typedef triple<_Tp> _Self;

public:
  /** Default constructor */
  inline triple() : _range(_Tp()), _typical(_Tp()) {}

  /** Construct a triple given an exact number */
  inline triple(const _Tp &a) : _range(a), _typical(a) {}

  /** Construct a triple {min : typical : max} */
  inline triple(const _Tp &min, _Tp typical, const _Tp &max)
      : _range(min, max), _typical(std::move(typical)) {
    assert(_range.contain(typical));
  }

  /** Construct a triple {min : typical : max} */
  inline triple(const interval<_Tp> &range, _Tp typical)
      : _range(range), _typical(std::move(typical)) {
    assert(_range.contain(typical));
  }

  // Compiler generated copy constructor and operator=() are fine.

  /** Construct a triple {min : typical : max} from another triple */
  template <typename _Up>
  inline triple(const triple<_Up> &T)
      : _range(T.range()), _typical(T.typical()) {
    assert(_range.contain(typical));
  }

  /** @return range. */
  inline const interval<_Tp> &range() const { return _range; }

  /** @return typical value. */
  inline const _Tp &typical() const { return _typical; }

  /** @return min value.. */
  inline const _Tp &min() const { return _range.lower(); }

  /** @return max value.. */
  inline const _Tp &max() const { return _range.upper(); }

  /**
   * @name Comparisons and Relations
   */

  //@{
  /** @return whether it is equal to T.
      We consider two triples are equal if their
      range and typical value are equal.
      Precondition: _Tp is EqualityComparable */
  inline bool operator==(const _Self &T) const {
    return _range == T._range && _typical == T._typical;
  }

  /** @return whether it is not equal T. @see operator==() */
  template <typename _Up> inline bool operator!=(const triple<_Up> &T) const {
    return !(*this == T);
  }

  /** @return whether this triple is left of T. An
      triple q is left of p iff q is completely less than
      p. Assume that less-than relation of_Tp admits partial ordering,
      i.e. irreflexive, anti-symmetric and transitive, then this
      relation also admits partial ordering. However, it
      does not admit strict weak ordering because equivalence is not
      transitive in general, unless all the triples interested are
      not overlapped. */
  template <typename _Up> inline bool left_of(const triple<_Up> &T) const {
    return _range.left_of(T._range);
  }

  /** @return whether it is right of T. @see left_of(). */
  template <typename _Up> inline bool right_of(const triple<_Up> &T) const {
    return _range.right_of(T._range);
  }

  /** @return whether this triple is less than I. An
      triple q is less than p iff the lower bound of q is less than the
      lower bound of q. If the lower bounds are equivalent, then q is less
      than p if the upper bound of q is less than p. */
  template <typename _Up> inline bool operator<(const triple<_Up> &T) const {
    return min() < T.min() || (!(T.min() < min()) && typical() < T.typical()) ||
           !(T.min() < min() || (T.typical() < typical() && max() < T.max()));
  }

  /** Greater than or equal to. @see operator<() */
  template <typename _Up> inline bool operator>=(const triple<_Up> &T) const {
    return !(*this < T);
  }

  /** Less than or equal to. @see operator<() */
  template <typename _Up> inline bool operator<=(const triple<_Up> &T) const {
    return !(T < *this);
  }

  /** Greater than. @see operator<() */
  template <typename _Up> inline bool operator>(const triple<_Up> &T) const {
    return T < *this;
  }

  /** @return whether a query q is within this triple. */
  inline bool contain(const _Tp &q) { return _range.contain(q); }

  /** @return whether it is overlapped with I. Note that overlap
      relation is reflexive, symmetric but not transitive. */
  template <typename _Up> inline bool overlap(const triple<_Up> &T) const {
    return _range.overlap(T._range);
  }

  //@}

  /**
   * @name Arithmetic Operations
   * Precondition: _Tp has corresponding arithmetic operations.
   */

  //@{
  /** Add. This operation is commutative and associative if the underlying
      operation of _Tp is commutative and associative. */
  inline _Self operator+(const _Self &b) const { return _Self(*this) += b; }

  /** Subtract. Note that (b - b) is not exactly 0 in general. */
  inline _Self operator-(const _Self &b) const { return _Self(*this) -= b; }

  /** Add and assign. */
  inline _Self &operator+=(const _Self &b) {
    _range += b._range, _typical += b._typical;
    return *this;
  }

  /** Subtract and assign.
      Note that (b - b) is not exactly 0 in general. */
  inline _Self &operator-=(const _Self &b) {
    _range -= b._range, _typical -= b._typical;
    return *this;
  }

  /** Multiply an exact number and assign */
  template <typename _Up> inline _Self &operator*=(const _Up &a) {
    _range *= a, _typical *= a; // note that "a" could be less than 0
    assert(_range.contain(_typical));
    return *this;
  }

  /** @todo multiplication of two triples */

  //@}

private:
  interval<_Tp> _range; /**< {min, max} */
  _Tp _typical;         /**< typical value */
};

/** Unary plus */
template <typename _Tp> inline triple<_Tp> operator+(const triple<_Tp> &b) {
  return b;
}

/** Unary minus */
template <typename _Tp> inline triple<_Tp> operator-(const triple<_Tp> &b) {
  return triple<_Tp>(-b.range(), -b.typical());
}

/** Print the data */
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &out, const triple<_Tp> &T) {
  out << "{" << T.min() << ", " << T.typical() << ", " << T.max() << "}";
  return out;
}

/** @} */
}

#endif
