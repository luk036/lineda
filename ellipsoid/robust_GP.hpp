#ifndef _ROBUST_GP_HPP
#define _ROBUST_GP_HPP

#include "posynomial.hpp"
#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

/**
 * Reference:
 *  S.P. Boyd, S.J. Kim and L.Vandenberghe and A. Hassibi.
 *  A Tutorial on Geometric Programming. Available at
 *  http://www.standford.edu/~boyd/gp_tutorial.html
 */

/**
 * Robust Geometric Programming in convex form.
 * A robust geometric program (RGP) is an optimization problem of the form
 *      minimize     max_{q \in Q} p_0(x)
 *      subject to   max_{q \in Q} p_i(x) <= 1, i = 1,...,m       (1)
 * where p_i are posynomial functions and x_i are the optimization
 * variables. Q represent the whole variation space.
 * (There is an implicit constraint that the variables are
 * positive, i.e., x_i > 0.) We refer to the problem (1) as a robust
 * geometric program in *standard* form. In a standard form GP, the
 * objective must be posynomial (and it must be minimized); the
 * equality constraints can only have the form of a monomial equal to
 * one (g_i(x) = 1) and must be represented by two inequality
 * constraints g_i(x) <= 1 and 1/g_i(x) <= 1.
 * The main trick to solving a robust GP efficiently is to convert it to a
 * nonlinear optimization problem, i.e., a problem with convex
 * objective and inequality constraint functions, and linear equality
 * constrains. Efficient solution methods for general convex
 * optimization problems are well developed [17]. The conversion of a
 * GP to a convex problem is based on a logarithmic change of
 * variables, and a logarithmic transformation of the objective and
 * constraint functions. In place of the original variables x_i, we
 * use their logarithms, y_i = log x_i (so x_i = exp(y_i)). Instead of
 * minimizing the objective p_0, we minimize its logarithm log p_0. We
 * replace the inequality constraints p_i <= 1 with log p_i <= 0. This
 * results in the problem
 *      minimize    max_{q \in Q} log p_0(exp(y))
 *      subject to  max_{q \in Q} log p_i(exp(y)) <= 0, i = 1,...,m,
 * with variable y = (y_1, ..., y_n). Here we use the notation exp(y),
 * where y is a vector, to mean componentwise exponentiation:
 * (exp(y))_i = exp(y_i).
 *
 * Note that Q is implicit to the data type _Tp.
 */
template <typename _Tp> class robust_GP {
  typedef std::valarray<double> Vec;

protected:
  std::vector<posynomial<_Tp> > _P; /**< a set of constraints and obj fct */
  mutable _Tp _Polarity;           /**< Polarity, for _Tp==AAF only */

public:
  /** @return the number of constraints */
  size_t num_constraints() const { return _P.size() - 1; }

  /** @return the evalution of max(f_j(exp(y))) */
  template <typename _Up>
  double operator()(const std::valarray<_Up> &y, int j) const {
    return max(_P[j](y));
  }

  /** @return the sub-gradient of max(f_j(exp(y))) */
  template <typename _Up>
  Vec gradient(const std::valarray<_Up> &y, int j) const {
    posynomial<double> Pd(convert(_P[j]));
    std::valarray<double> yd(convert(y));
    return Pd.gradient(yd);
  }

protected:
  /** Constructor */
  robust_GP();

  /** Destructor */
  ~robust_GP();

private:
  // Degenerate copy constructor and assignment operator
  robust_GP(const robust_GP<_Tp> &);
  robust_GP<_Tp> &operator=(const robust_GP<_Tp> &);

  double max(const _Tp &a) const;
  posynomial<double> convert(const posynomial<_Tp> &c) const;
  template <typename _Up>
  std::valarray<double> convert(const std::valarray<_Up> &y) const;
};

#endif
