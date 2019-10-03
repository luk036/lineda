#ifndef _ELLIPSOID_HPP
#define _ELLIPSOID_HPP

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

/**
 * References: ???
 */

/** Assign A*b to Ab, and then return dot(b, Ab) */
template <class Matrix, class Vec>
inline double mult_dot(const Matrix &A, const Vec &b, Vec &Ab) {
  double dot = 0.;
  const size_t n = b.size();
  for (size_t i = 0; i < n; ++i) {
    Ab[i] = 0.;
    for (size_t j = 0; j < n; ++j) {
      Ab[i] += A[i][j] * b[j];
    }
    dot += b[i] * Ab[i];
  }
  return dot;
}

/**
 * -- Ellipsoid method for sovling convex programming:
 *     minimize     fct(x,0)
 *     subject to   fct(x,j) <= 0
 *       where fct(x,0) and fct(x,j)'s are convex
 *
 * input
 * 	E(Ae,x)       ellipsoid containing x*
 *      max_it        maximum number of iterations
 *      tol           error tolerance
 *      fct           function object that has the following members:
 *          fct.num_constraints() returns the number of constraints
 *          fct(x,0)              evaluate the objective function
 *          fct(x,j)              evaluate the j-th constraints
 *          fct.gradient(x,0)     returns gradient of objective function
 *          fct.gradient(x,j)     returns gradient of the j-th constraints
 *
 * output
 *      x             solution vector
 *      flag          0 = solution found to tolerance
 *                    1 = no convergence given max_it
 *                    2 = no feasible sol'n
 *                    3 = dot product becomes negative
 */
template <class F, class Matrix, class Vector>
int ellipsoid(F &fct, Matrix &Ae, Vector &x, int max_it, double tol) {
  const size_t n = x.size();
  const size_t m = fct.num_constraints();

  const double n2 = double(n) * n;
  const double alpha = n2 / (n2 - 1.);
  const double beta = 2. / (n + 1.);

  std::valarray<double> g(n);   // gradient
  std::valarray<double> Aeg(n); // Ae * g
  double error = 1e10;
  for (int iter = 1; iter <= max_it; ++iter) {
    bool feasible = true;
    for (size_t j = 1; j <= m; ++j) {
      double fj = fct(x, j);
      if (fj > 0) { //  x infeasible
        g = fct.gradient(x, j);
        const double dotprod = mult_dot(Ae, g, Aeg);
        if (dotprod < 0) {
          std::cout << "dotprod = " << dotprod << std::endl;
          return 3;
        }
        error = sqrt(dotprod);
        if (fj - error > 0)
          return 2; // no feasible sol'n
        feasible = false;
        break;
      }
    }

    if (feasible) { //  x feasible
      fct(x, 0);    // call it anyway in order to determine the polarity
                    // @todo move it to the specific code
      g = fct.gradient(x, 0);
      const double dotprod = mult_dot(Ae, g, Aeg);
      if (dotprod < 0) {
        std::cout << "dotprod = " << dotprod << std::endl;
        return 3;
      }
      error = sqrt(dotprod);
      if (error < tol)
        return 0;
    }

    // Find min. volume ellipsoid containing feasible sol'ns.
    for (size_t i = 0; i < n; ++i) {
      Ae[i] /= error;
    }

    for (size_t i = 0; i < n; ++i) {
      x[i] -= Ae[i] / (n + 1.);
      for (size_t j = 0; j < n; ++j) {
        Ae[i][j] -= beta * Ae[i] * Ae[j];
        Ae[i][j] *= alpha;
      }
    }
  }

  if (error > tol)
    return 1;
  return 0;
}

// END of ellipsoid

#endif
