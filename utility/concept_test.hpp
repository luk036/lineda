#ifndef LINEDA_CONCEPT_TEST_HPP
#define LINEDA_CONCEPT_TEST_HPP

// $Id: concept_test.hpp 211 2005-10-17 12:23:48Z luk $
/**
 * @defgroup utility Common Utility
 * @ingroup lineda
 * This module is responsible for ...
 */

#include <functional>

namespace lineda {
/**
 * @addtogroup utility
 * @{
 */

/** @return whether copy constructor is verified OK.
    Pre-requisites: _Tp is copyable and equality comparable. */
template <typename _Tp> inline bool copy_ok(const _Tp &a) {
  return _Tp(a) == a;
}

/** @return whether assignment operator is verified OK.
    Pre-requisites: _Tp is assingable and equality comparable. */
template <typename _Tp> inline bool assign_ok(const _Tp &a) {
  _Tp b;
  return (b = a) == a;
}

/** @return whether reverse operator is verified OK.
    Pre-requisites: _Tp is copyable and equality comparable. */
template <typename _Tp> inline bool reverse_ok(const _Tp &a) {
  _Tp b(a);
  b.reverse();
  b.reverse();
  return b == a;
}

/** Reflexive relation: f(a, a) is always true.
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F> inline bool reflexive_ok(const _Tp &a, _F f) {
  return f(a, a);
}

/** Reflexive relation: f(a, a) is always true.
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool reflexive_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return reflexive_ok(a, f) && reflexive_ok(b, f) && reflexive_ok(c, f);
}

/** Irreflexive relation: f(a, a) is always false.
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool irreflexive_ok(const _Tp &a, _F f) {
  return !f(a, a);
}

/** Irreflexive relation: f(a, a) is always false.
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool irreflexive_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return irreflexive_ok(a, f) && irreflexive_ok(b, f) && irreflexive_ok(c, f);
}

/** Symmetric relation. f(a, b) implies f(b, a)
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool symmetric_ok(const _Tp &a, const _Tp &b, _F f) {
  return f(a, b) == f(b, a);
}

/** Symmetric relation. f(a, b) implies f(b, a)
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool symmetric_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return symmetric_ok(a, b, f) && symmetric_ok(b, c, f) &&
         symmetric_ok(c, a, f);
}

/** Anti-symmetric relation. f(a, b) implies !f(b, a)
    (Note: f(a, b) and f(b, a) could both be false.
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool anti_symmetric_ok(const _Tp &a, const _Tp &b, _F f) {
  return !(f(a, b) && f(b, a));
}

/** Anti-symmetric relation. f(a, b) implies !f(b, a)
    Pre-requisites: f is a binary predicate function. */
template <typename _Tp, class _F>
inline bool anti_symmetric_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return anti_symmetric_ok(a, b, f) && anti_symmetric_ok(b, c, f) &&
         anti_symmetric_ok(c, a, f);
}

/** Transitive relation: f(a, b) and f(b, c) implies f(a, c).
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F>
inline bool transitive_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return !(f(a, b) && f(b, c) && !f(a, c)) &&
         !(f(b, c) && f(c, a) && !f(b, a)) &&
         !(f(c, a) && f(a, b) && !f(c, b)) &&
         !(f(a, c) && f(c, b) && !f(a, b)) &&
         !(f(c, b) && f(b, a) && !f(c, a)) && !(f(b, a) && f(a, c) && !f(b, c));
}

/** Equivalent relation: f is called equivalent iff it is
    reflexive, symmetry and transitive.
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F>
inline bool equivalent_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return reflexive_ok(a, b, c, f) && symmetric_ok(a, b, c, f) &&
         transitive_ok(a, b, c, f);
}

/** @return whether operator==() is verified OK. */
template <typename _Tp>
inline bool equal_to_ok(const _Tp &a, const _Tp &b, const _Tp &c) {
  return equivalent_ok(a, b, c, std::equal_to<_Tp>());
}

/** Partial order relation: f is called partial order
    iff it is irreflexive, anti-symmetry and transitive.
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F>
inline bool partial_order_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return irreflexive_ok(a, b, c, f) && anti_symmetric_ok(a, b, c, f) &&
         transitive_ok(a, b, c, f);
}

/** Total order relation: f is called total order
    iff it is partial order and f(a, b) || f(b, a)
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F>
inline bool total_order_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return partial_order_ok(a, b, c, f) && (f(a, b) || f(b, a)) &&
         (f(b, c) || f(c, b)) && (f(c, a) || f(a, c));
}

/** Functor class. Apdator class for eqivalent-to relation of f.
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F> class equivalent_to {
private:
  _F f;

public:
  equivalent_to(const _F &g) : f(g) {}
  inline bool operator()(const _Tp &a, const _Tp &b) const {
    return !f(a, b) && !f(b, a);
  }
};

/** Strictly weak order relation: f is called strictly weak order
    iff it is partial order and equivalence is transitive.
    Pre-requisites: f is a binary predicate function */
template <typename _Tp, class _F>
inline bool strictly_weak_order_ok(const _Tp &a, const _Tp &b, const _Tp &c,
                                   _F f) {
  return partial_order_ok(a, b, c, f) &&
         transitive_ok(a, b, c, equivalent_to<_Tp, _F>(f));
}

/** @return whether operator<() is verified OK. */
template <typename _Tp>
inline bool less_ok(const _Tp &a, const _Tp &b, const _Tp &c) {
  return strictly_weak_order_ok(a, b, c, std::less<_Tp>());
}

/** Comutative. f(a, b) == f(b, a)
    Pre-requisites: f is a binary function and _Tp is equaltiy comparable */
template <typename _Tp, class _F>
inline bool commutative_ok(const _Tp &a, const _Tp &b, _F f) {
  return f(a, b) == f(b, a);
}

/** Comutative. f(a, b) == f(b, a)
    Pre-requisites: f is a binary function and _Tp is equaltiy comparable */
template <typename _Tp, class _F>
inline bool commutative_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return commutative_ok(a, b, f) && commutative_ok(b, c, f) &&
         commutative_ok(c, a, f);
}

/** Associative: f(a, f(b, c)) == f(f(a, b), c)
    Pre-requisites: f is a binary function that returns same _Tp and
    _Tp is equality comparable*/
template <typename _Tp, class _F>
inline bool associative_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return f(a, f(b, c)) == f(f(a, b), c) && f(b, f(c, a)) == f(f(b, c), a) &&
         f(c, f(a, b)) == f(f(c, a), b) && f(a, f(c, b)) == f(f(a, c), b) &&
         f(c, f(b, a)) == f(f(c, b), a) && f(b, f(a, c)) == f(f(b, a), c);
}

/** Distributive: a * (b + c) == (a * b) + (a * c) and
    (b + c) * a == (b * a) + (c * a).
    Pre-requisites: f1 and f2 are both binary function that returns
    same _Tp and _Tp is equality comparable */
template <typename _Tp, class _F1, class _F2>
inline bool distributive_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F1 f1,
                            _F2 f2) {
  return f1(a, f2(b, c)) == f2(f1(a, b), f1(a, c)) &&
         f1(b, f2(c, a)) == f2(f1(b, c), f1(b, a)) &&
         f1(c, f2(a, b)) == f2(f1(c, a), f1(c, b)) &&
         f1(a, f2(c, b)) == f2(f1(a, c), f1(a, b)) &&
         f1(c, f2(b, a)) == f2(f1(c, b), f1(c, a)) &&
         f1(b, f2(a, c)) == f2(f1(b, a), f1(b, c)) &&
         f1(f2(b, c), a) == f2(f1(b, a), f1(c, a)) &&
         f1(f2(c, a), b) == f2(f1(c, b), f1(a, b)) &&
         f1(f2(a, b), c) == f2(f1(a, c), f1(b, c)) &&
         f1(f2(c, b), a) == f2(f1(c, a), f1(b, a)) &&
         f1(f2(b, a), c) == f2(f1(b, c), f1(a, c)) &&
         f1(f2(a, c), b) == f2(f1(a, b), f1(c, b));
}

/** @return whether operator+() is verified OK. */
template <typename _Tp>
inline bool plus_ok(const _Tp &a, const _Tp &b, const _Tp &c) {
  return commutative_ok(a, b, c, std::plus<_Tp>()) &&
         associative_ok(a, b, c, std::plus<_Tp>());
}

/** @return whether operator*() is verified OK. */
template <typename _Tp>
inline bool multiplies1_ok(const _Tp &a, const _Tp &b, const _Tp &c) {
  return commutative_ok(a, b, c, std::multiplies<_Tp>()) &&
         associative_ok(a, b, c, std::multiplies<_Tp>()) &&
         distributive_ok(a, b, c, std::multiplies<_Tp>(), std::plus<_Tp>());
}

/** @return whether operator*() is verified OK. */
template <typename _Tp>
inline bool multiplies2_ok(const _Tp &a, const _Tp &b, const _Tp &c) {
  return associative_ok(a, b, c, std::multiplies<_Tp>()) &&
         distributive_ok(a, b, c, std::multiplies<_Tp>(), std::plus<_Tp>());
}

/** (_Tp, f) is semi-group iff _Tp is close and f is associative.
    Pre-requisites: f is a binary function that returns same _Tp and
    _Tp is equality comparable */
template <typename _Tp, class _F>
inline bool semi_group_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return associative_ok(a, b, c, f);
}

/** (_Tp, f) is a group iff f is associative and there exists an identity
    element id such that f(id, a) == f(a, id) == a
    Pre-requisites: f is a binary function that returns same _Tp and
    _Tp is equality_comparable */
template <typename _Tp, class _F>
inline bool group_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return semi_group_ok(a, b, c, f);
}

/** (_Tp, f) is an Abel group iff (_Tp, f) is a group and f is communicative.
    Pre-requisites: f is a binary function that returns same _Tp and
    _Tp is equality_comparable */
template <typename _Tp, class _F>
inline bool abel_group_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return group_ok(a, b, c, f) && commutative_ok(a, b, c, f);
}

/** Ring.
    Pre-requisites: f1 and f2 are both binary functions that return
    same _Tp. In addition _Tp is equality comparable */
template <typename _Tp, class _F1, class _F2>
inline bool ring_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F1 f1, _F2 f2) {
  return abel_group_ok(a, b, c, f1) && semi_group_ok(a, b, c, f2) &&
         distributive_ok(a, b, c, f1, f2);
}

/** Field.
    Pre-requisites: f1 and f2 are both binary function that returns
    same _Tp and _Tp is equality comparable */
template <typename _Tp, class _F1, class _F2>
inline bool field_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F1 f1, _F2 f2) {
  return ring_ok(a, b, c, f1, f2);
}

/** Verify if "f" is a linear function. */
template <typename _Tp, class _F>
inline bool linear_ok(const _Tp &a, const _Tp &b, _F f) {
  return f(a + b) == f(a) + f(b);
  // @todo take care round-off error
}

/** Verify if "f" is a (strictly) monotone increasing function.
    Note that an inverse function of f exists if f is s.m.i */
template <typename _Tp, class _F>
inline bool monotone_increasing_ok(const _Tp &a, const _Tp &b, _F f) {
  return (a < b) == (f(a) < f(b));
}

/** Verify if "f" is a convex function.
    Note that the local minimum is also the global minimum if
    f is a convex function. */
template <typename _Tp, class _F>
inline bool convex_ok(const _Tp &a, const _Tp &b, _F f) {
  return f(0.4 * a + 0.6 * b) <= 0.4 * f(a) + 0.6 * f(b);
}

/** Verify if <_Tp, dist> is a metric. */
template <typename _Tp, class _F>
inline bool metric_ok(const _Tp &a, const _Tp &b, _F dist) {
  const double d = dist(a, b);
  return d >= 0                  // non-negativity
         && (d == 0) == (a == b) // non-degeneracy
         && d == dist(b, a);     // symmetry
}

/** Verify if <_Tp, dist> is a metric. */
template <typename _Tp, class _F>
inline bool metric_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F dist) {
  return dist(a, c) <= dist(a, b) + dist(b, c) // triangle inequality
         && metric_ok(a, b, dist) && metric_ok(b, c, dist) &&
         metric_ok(a, c, dist);
}

/** Verify if "f" is a norm function. */
template <typename _Tp, class _F>
inline bool norm_ok(const _Tp &a, const _Tp &b, _F norm) {
  const double fa = norm(a);
  const double fb = norm(b);
  return fa >= 0 && fb >= 0         // non-negativity
         && (fa == 0) == (a == 0)   // non-degeneracy
         && (fb == 0) == (b == 0)   // non-degeneracy
         && norm(-2 * a) == 2 * fa  // positive homogeneity
         && norm(-2 * b) == 2 * fb  // positive homogeneity
         && norm(a + b) <= fa + fb; // triangle inequality
}

/** Verify if "f" is a norm function. */
template <typename _Tp, class _F>
inline bool norm_ok(const _Tp &a, const _Tp &b, const _Tp &c, _F f) {
  return norm_ok(a, b, f) && norm_ok(b, c, f) && norm_ok(c, a, f);
}

/** Verify if <_Tp, inner> is an (real) inner product space. */
template <typename _Tp, class _F>
inline bool inner_product_ok(const _Tp &v, const _Tp &w, _F inner) {
  const double d = inner(v, v);
  const double e = inner(w, w);
  const _Tp z = _Tp(0);
  return d >= 0 && e >= 0                          // non-negativity
         && (d == 0) == (v == z)                   // non-degeneracy
         && (e == 0) == (w == z)                   // non-degeneracy
         && inner(-2. * v, w) == -2. * inner(v, w) // multiplicity
         && inner(-2. * w, v) == -2. * inner(w, v) // multiplicity
         && inner(v, w) == inner(w, v);            // symmetry
}

/** Verify if <_Tp, inner> is an (real) inner product space. */
template <typename _Tp, class _F>
inline bool inner_product_ok(const _Tp &v, const _Tp &w, const _Tp &u,
                             _F inner) {
  return inner(v, w + u) == inner(v, w) + inner(v, u) // distributivity
         && inner_product_ok(v, w, inner) && inner_product_ok(w, u, inner) &&
         inner_product_ok(v, u, inner);
}

/** @} */
}

#endif
