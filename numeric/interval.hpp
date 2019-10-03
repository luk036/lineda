#ifndef NUMERIC_INTERVAL_HPP
#define NUMERIC_INTERVAL_HPP

// $Id: interval.hpp 891 2006-03-25 06:17:03Z luk $
/**
 * @defgroup numeric Numerical Package
 * @ingroup lineda
 * This module is responsible for ...
 */

#include <boost/concept_check.hpp>
#include <cassert>
#include <iosfwd>
#include <utility>

namespace numeric {

/**
 * @addtogroup numeric
 * @{
 */

/**
 * An interval class. Define [lower .. upper) be an interval where
 * both lower, upper are of type _Tp and !(upper < lower).
 * Note that the point "upper" does not belong to the interval such
 * that [1..4] and [4..9] are not overlapped.
 *
 * Pre-requisite: _Tp is lessThanComparable.
 */
template <typename _Tp> class interval {
  BOOST_CLASS_REQUIRE(_Tp, boost, LessThanComparableConcept);
  typedef interval<_Tp> _Self;

public:
  /** Default constructor */
  inline interval() : _lower(_Tp()), _upper(_Tp()) {}

  /** Create an interval given an "exact" number */
  inline interval(const _Tp &a) : _lower(a), _upper(a) {}

  /** Create an interval [lower .. upper] */
  inline interval(_Tp lower, _Tp upper)
      : _lower(std::move(lower)), _upper(std::move(upper)) {
    assert(!(upper < lower));
  }

  /** Create an interval from another interval */
  template <typename _Up>
  inline interval(const interval<_Up> &I)
      : _lower(I.lower()), _upper(I.upper()) {}

  /** Assign an interval from another interval */
  inline _Self &operator=(const _Self &I) {
    parallel(_lower = I.lower(), _upper = I.upper());
    return *this;
  }

  // Compiler generated copy constructor and operator=() are fine.

  /** @return lower bound. */
  inline const _Tp &lower() const { return _lower; }

  /** @return upper bound. */
  inline const _Tp &upper() const { return _upper; }

  /** @return whether this interval is degenerated to a single point */
  inline bool is_exact() const { return lower() == upper(); }

  /** Create a random interval. */
  static _Self random();

  /**
   * @name Comparisons and Relations
   */

  //@{
  /** @return whether this interval is left of I. An
      interval q is left of p iff q is completely less than
      p. Assume that less-than relation of_Tp admits partial ordering,
      i.e. irreflexive, anti-symmetric and transitive, then this
      relation also admits partial ordering. However, it
      does not admit strict weak ordering because equivalence is not
      transitive in general, unless all the intervals interested are
      not overlapped. */
  template <typename _Up> inline bool left_of(const interval<_Up> &I) const {
    return upper() < I.lower();
  }

  /** @return whether it is right of I. @see left_of(). */
  template <typename _Up> inline bool right_of(const interval<_Up> &I) const {
    return lower() > I.upper();
  }

  /** @return whether query q is within this interval. */
  template <typename _Up> inline bool contain(const _Up &q) const {
    return !(q < lower() || upper() < q);
  }

  /** @return whether interval I is completely within this interval. */
  template <typename _Up> inline bool contain(const interval<_Up> &I) const {
    return !(I.lower() < lower() || upper() < I.upper());
  }

  /** @return whether it is overlapped with I. Note that overlap
      relation is reflexive, symmetric but not transitive. */
  template <typename _Up> inline bool overlap(const interval<_Up> &I) const {
    return !(left_of(I) || I.left_of(*this));
  }

  /** @return the intersection of this interval and I.
      Precondition: two intervals are overlapped. */
  _Self intersect(const _Self &I) const {
    assert(overlap(I));
    return _Self(std::max(lower(), I.lower()), std::min(upper(), I.upper()));
  }

  /** @return the intersection of this interval and I. */
  _Self intersect2(const _Self &I) const {
    const _Tp l = std::max(lower(), I.lower());
    const _Tp u = std::min(upper(), I.upper());
    if (l <= u)
      return _Self(l, u);
    return _Self((l + u) / 2);
  }

  /** @return the hull (bounding "box") of this interval and I. */
  _Self hull(const _Self &I) const {
    return _Self(std::min(lower(), I.lower()), std::max(upper(), I.upper()));
  }

  /** @return the gap between this interval and I. */
  _Self gap(const _Self &I) const {
    if (overlap(I))
      return interval<_Tp>(); // ???
    return _Self(std::min(upper(), I.upper()), std::max(lower(), I.lower()));
  }

  //@}

  /**
   * @name Arithmetic Operations
   * Precondition: _Tp has corresponding arithmetic operations.
   */

  //@{
  /** Add. This operation is commutative and associative if the underlying
      operation of _Tp is commutative and associative. */
  inline _Self operator+(const _Self &b) const {
    return interval<_Tp>(*this) += b;
  }

  /** Subtract. Note that (b - b) is not exactly 0 in general. */
  inline interval<_Tp> operator-(const interval<_Tp> &b) const {
    return interval<_Tp>(lower() - b.upper(), upper() - b.lower());
  }

  /** Add and assign. */
  template <typename _Up> inline _Self &operator+=(const interval<_Up> &b) {
    parallel(_lower += b.lower(), _upper += b.upper());
    return *this;
  }

  /** Add and assign. */
  template <typename _Up> inline _Self &operator+=(const _Up &b) {
    parallel(_lower += b, _upper += b);
    return *this;
  }

  /** Substract and assign. */
  template <typename _Up> inline _Self &operator-=(const _Up &b) {
    parallel(_lower -= b, _upper -= b);
    return *this;
  }

  /** Subtract and assign.
      Note that (b - b) is not exactly 0 in general. */
  inline _Self &operator-=(const _Self &b) { return *this = *this - b; }

  /** Multiply an exact number and assign.
      @todo Specialize for unsigned integers so that don't need to check. */
  template <typename _Up> inline _Self &operator*=(const _Up &a) {
    parallel(_lower *= a, _upper *= a); // note that "a" could be less than 0
    if (_upper < _lower)
      std::swap(_lower, _upper);
    return *this;
  }

  /** Divide by an exact number and assign.
      @todo Specialize for unsigned integers so that don't need to check. */
  template <typename _Up> inline _Self &operator/=(const _Up &a) {
    parallel(_lower /= a, _upper /= a); // note that "a" could be less than 0
    if (_upper < _lower)
      std::swap(_lower, _upper);
    return *this;
  }

  /** Multiply an unsigned int and assign. */
  inline _Self &operator*=(unsigned int a) {
    parallel(_lower *= a, _upper *= a);
    return *this;
  }

  /** Divide by an exact number and assign. */
  inline _Self &operator/=(unsigned int a) {
    parallel(_lower /= a, _upper /= a);
    return *this;
  }

  /** @return multiplication of two intervals.
      Note that + does not distribute over * in general. */
  inline interval<_Tp> operator*(const interval<_Tp> &I) const {
    interval<_Tp> I1(*this), I2(*this);
    parallel(I1 *= I.lower(), I2 *= I.upper());
    return I1.hull(I2);
  }

  /** @return division of two intervals.
      Note that + does not distribute over / in general. */
  inline interval<_Tp> operator/(const interval<_Tp> &I) const {
    interval<_Tp> I1(*this), I2(*this);
    parallel(I1 /= I.lower(), I2 /= I.upper());
    return I1.hull(I2);
  }

  /** @return multiplication of two intervals */
  inline interval<_Tp> &operator*=(const interval<_Tp> &I) {
    return *this = *this * I;
  }

  /** @return multiplication of two intervals */
  inline interval<_Tp> &operator/=(const interval<_Tp> &I) {
    return *this = *this / I;
  }

  /** @return the range */
  inline _Tp range() const { return upper() - lower(); }

  /** Apply a unary function f to the intervals. Pre-condition: f is a
      monotonic increasing function, i.e. f(a) < f(b) iff a < b. */
  template <class _Function, typename _Up>
  interval<_Up> apply(_Function __f) const {
    return interval<_Up>(__f(lower()), __f(upper()));
  }

  //@}

private:
  // Just indicate that two operations can be executed in parallel
  template <class _Up1, class _Up2>
  inline void parallel(const _Up1 &, const _Up2 &) const {}

private:
  _Tp _lower; /**< lower bound */
  _Tp _upper; /**< upper bound */
};

/** @return whether it is equal to I.
    We consider two intervals are equal if their
    lower bounds and upper bounds are equal.
    Precondition: _Tp is EqualityComparable */
template <typename _Tp, typename _Up>
inline bool operator==(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return lhs.lower() == rhs.lower() && lhs.upper() == rhs.upper();
}

/** @return whether it is not equal p. @see operator==() */
template <typename _Tp, typename _Up>
inline bool operator!=(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return !(lhs == rhs);
}

/** @return whether this interval is lexicographically less than I. An
    interval q is less than p iff the lower bound of q is less than the
    lower bound of q. If the lower bounds are equivalent, then q is less
    than p if the upper bound of q is less than p. */
template <typename _Tp, typename _Up>
inline bool operator<(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return lhs.lower() < rhs.lower() ||
         (!(rhs.lower() < lhs.lower()) && lhs.upper() < rhs.upper());
}

/** Greater than or equal to. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator>=(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return !(lhs < rhs);
}

/** Less than or equal to. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator<=(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return !(rhs < lhs);
}

/** Greater than. @see operator<() */
template <typename _Tp, typename _Up>
inline bool operator>(const interval<_Tp> &lhs, const interval<_Up> &rhs) {
  return rhs < lhs;
}

/** Unary plus */
template <typename _Tp> inline interval<_Tp> operator+(const interval<_Tp> &b) {
  return b;
}

/** Unary minus ??? */
template <typename _Tp> inline interval<_Tp> operator-(const interval<_Tp> &b) {
  return interval<_Tp>(-b.upper(), -b.lower());
}

// Non-member functions

/** Print the data */
template <typename _Tp>
inline std::ostream &operator<<(std::ostream &out, const interval<_Tp> &I) {
  out << "[" << I.lower() << " .. " << I.upper() << "]";
  return out;
}

/** @return the max of two intervals. Note that it is not equal to
    (I1 < I2)? I2 : I1 */
template <typename _Tp>
inline interval<_Tp> max(const interval<_Tp> &I1, const interval<_Tp> &I2) {
  return interval<_Tp>(std::max(I1.lower(), I2.lower()),
                       std::max(I1.upper(), I2.upper()));
}

/** @return the min of two intervals. Note that it is not equal to
    (I1 < I2)? I1 : I2 */
template <typename _Tp>
inline interval<_Tp> min(const interval<_Tp> &I1, const interval<_Tp> &I2) {
  return interval<_Tp>(std::min(I1.lower(), I2.lower()),
                       std::min(I1.upper(), I2.upper()));
}

/** @return an interval given two numbers */
template <typename _Tp> inline interval<_Tp> make_interval(_Tp x, _Tp y) {
  if (y < x)
    std::swap(x, y);
  return interval<_Tp>(x, y);
}

// Specialization

/** @return multiplication of two intervals.
    If _Tp is unsigned, the distributive law is satisfied. */
template <>
inline interval<unsigned int> interval<unsigned int>::
operator*(const interval<unsigned int> &I) const {
  return interval<unsigned int>(lower() * I.lower(), upper() * I.upper());
}

/** @return division of two intervals.
    If _Tp is unsigned, the distributive law is satisfied. */
template <>
inline interval<unsigned int> interval<unsigned int>::
operator/(const interval<unsigned int> &I) const {
  return interval<unsigned int>(lower() / I.upper(), upper() / I.lower());
}

/** @} */
}

/** @return the minimum distance of two intervals */
template <typename _Tp>
inline _Tp min_dist(const numeric::interval<_Tp> &I1,
                    const numeric::interval<_Tp> &I2) {
  return (I1.gap(I2)).range();
}

/** @return the maximum distance of two intervals */
template <typename _Tp>
inline _Tp max_dist(const numeric::interval<_Tp> &I1,
                    const numeric::interval<_Tp> &I2) {
  return (I1.hull(I2)).range();
}

#endif
