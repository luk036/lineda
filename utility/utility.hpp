#ifndef LINEDA_UTILITY_HPP
#define LINEDA_UTILITY_HPP

#include <cassert>
#include <cstdlib>

/**
 * @addtogroup utility
 * @{
 */

namespace lineda {

/** To suppress unused variable warning */
template <typename Tp> void ignore(const Tp &) {}

/** @return factorial n!. Time complexity: O(n).
    n! = 1*2*3*...*n   for n>0
    0! = 0
    n! = n * (n-1)!    recursively definition
 */
template <typename _Tp> inline _Tp factorial(size_t n) {
  if (n < 2)
    return 1.0;

  _Tp res = 1.0;
  for (size_t i = 1; i <= n; ++i) {
    res *= i;
  }
  return res;
}

/** @return the binomial_coeff C(n,k). The
    number of k-combinations of an n-set is also equal to C(n,k).
      C(n,k) = n!/[k!(n-k)!].
      C(n,k) = C(n-1,k) + C(n-1, k-1)   recursively definition
    For example, C(4,2) = 6
    Pre-condition: n >= k. Time complexity: O(n)*/
template <typename _Tp> inline _Tp binomial_coeff(size_t n, size_t k) {
  assert(n >= k);

  if (n == 0)
    return 0.0;
  if (k < n / 2)
    k = n - k;

  _Tp res = 1.0;
  for (size_t i = 1; i <= n - k; ++i) {
    res *= k + i;
    res /= i;
  }

  return res;
}

/** @return the Catalan number C(n). The number
    of extended binary trees with n internal nodes is given by the
    nth Catalan numbe (2n)!/[(n+1)!n!] which, for n=1,2,...,15, has
    the values 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786,
    208012, 742900, 2674440, 9694854.
    Time complexity: O(n).
    Reference: http://www.research.att.com/~njas/sequences/A000108
*/
template <typename _Tp> inline _Tp catalan_number(size_t n) {
  if (n < 2)
    return 1;

  _Tp C = 1;
  for (size_t i = 2; i <= n; ++i) {
    C *= 4 * i - 2;
    C /= i + 1;
  }
  return C;
}

/** @return the fabonacci number f(n).
    Time complexity: O(n).
*/
template <typename _Tp> inline _Tp fibonacci(size_t n) {
  if (n < 2)
    return n;

  _Tp f(1), f2(1), f1(1);
  for (size_t i = 2; i <= n; ++i) {
    f += f2;
    f2 = f1;
    f1 = f;
  }
  return f;
}

/** @return the little Schr{\"o}der number s(n).
    Time complexity: O(n).
    Reference: http://www.research.att.com/~njas/sequences/A001003
    @todo: there are other ways to generate the sequence (see the
    reference), try other methods and compare the performance.
*/
template <typename _Tp> inline _Tp schroder(size_t n) {
  if (n < 2)
    return 1;
  /* Recurrence: a(1) = a(2) = 1; */
  /* for n > 1, (n+1)a(n+1) = 3(2n-1)a(n)-(n-2)a(n-1).*/
  _Tp s(1), s1(1), s2(1);
  for (size_t i = 2; i < n; ++i) {
    s *= 6 * i - 3;
    s -= (i - 2) * s2;
    s /= (i + 1);
    s2 = s1;
    s1 = s;
  }
  return s;
}

/** @return the Baxter number B(n).
    Time complexity: O(n).
    Reference: http://www.research.att.com/~njas/sequences/A001181
    @todo: there are other ways to generate the sequence (see the
    reference), try other methods and compare the performance.
*/
template <typename _Tp> inline _Tp baxter(size_t n) {
  if (n == 1)
    return 0;
  if (n == 2)
    return 1;
  if (n == 3)
    return 2;
  if (n == 4)
    return 6;

  _Tp B, B1, B2, B3;
  B = 6;
  B1 = 6;
  B2 = 2;
  B3 = 1;

  for (size_t i = 4; i < n; ++i) {
    B *= 2 * (i + 1) * (((9 * i + 3) * i - 4) * i + 4);
    B += B2 * (3 * i - 1) * (i - 2) * ((15 * i - 5) * i - 14);
    B += B3 * 8 * (3 * i + 1) * (i - 2) * (i - 2) * (i - 3);
    B /= (i + 1) * (i + 2) * (i + 3) * (3 * i - 2);
    B3 = B2;
    B2 = B1;
    B1 = B;
  }
  return B;
}
}

#endif
