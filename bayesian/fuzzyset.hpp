#ifndef LINEDA_FUZZY_SET_HPP
#define LINEDA_FUZZY_SET_HPP

#include <algorithm>
#include <cmath>

/**
 * References:
 * [1] David M. Bourg and Glenn Seemann. AI for game developers,
 * O'Reilly, 2005, TP311.52/B772
 */

/** Fuzzy Logic */
class fzlogic {
public:
  /** Default Constructor */
  fzlogic() : _degree(0.) {}

  /** Create a fuzzy logic with degree d */
  explicit fzlogic(double d) : _degree(d) {}

  /** Destructor */
  ~fzlogic() {}

  // Compiler generated copy constructor and operator=() are fine

  /** @return degree of membership */
  double degree() const { return _degree; }

  /** Compiler generated copy constructor and operator=() are fine */
private:
  double _degree; /**< degree of membership, btw 0 to 1 */
};

/** Fuzzy sub-set */
template <class Membershipfct> class fzset {
public:
  /** Create a fuzzy set with membershipfct */
  explicit fzset(const Membershipfct &mu) : _mu(mu) {}

  /** Destructor */
  ~fzset() {}

  // Compiler generated copy constructor and operator=() are fine.

  /** @return degree of membership of x in form of fuzzy logic */
  fzlogic operator()(double x) const { return fzlogic(_mu(x)); }

private:
  Membershipfct _mu; /**< membership function */
};

/** Logical operators */

/** Disjunction */
inline fzlogic operator||(const fzlogic &A, const fzlogic &B) {
  return fzlogic(std::max(A.degree(), B.degree()));
}

/** Conjunction */
inline fzlogic operator&&(const fzlogic &A, const fzlogic &B) {
  return fzlogic(std::min(A.degree(), B.degree()));
}

/** Negation */
inline fzlogic operator!(const fzlogic &A) { return fzlogic(1. - A.degree()); }

/** Mixed operations */

/** Disjunction */
inline fzlogic operator||(bool a, const fzlogic &B) {
  if (a)
    return fzlogic(1.);
  return B;
}

/** Disjunction */
inline fzlogic operator||(const fzlogic &B, bool a) { return a || B; }

/** Conjunction */
inline fzlogic operator&&(bool a, const fzlogic &B) {
  if (a)
    return B;
  return fzlogic(0.);
}

/** Conjunction */
inline fzlogic operator&&(const fzlogic &B, bool a) { return a && B; }

/** Hedges. Hedge functions are sometimes used to modify the degree of
membership returned by a membership function. The idea behind hedges
is to provide additional linguistic constructs that you can use in
conjunction with other logical operations. Two common hedges are VERY
and NOT_VERY, which are defined as follows. */

inline fzlogic VERY(const fzlogic &A) {
  return fzlogic(A.degree() * A.degree());
}

inline fzlogic NOT_VERY(const fzlogic &A) { return fzlogic(sqrt(A.degree())); }

#endif
