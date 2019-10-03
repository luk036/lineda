#include "newton.hpp"
#include "matrix2.hpp"
#include "polynomial.hpp"
#include "vector2.hpp"
#include <complex>

namespace numeric {

inline double norm(double x) { return fabs(x); }

template <typename Tp, class F>
bool newton_solve(Tp &x, const F &f, int max_it, double tol) {
  // Initialization
  for (int iter = 1; iter <= max_it; ++iter) {
    const Tp fx = f(x);
    if (norm(fx) <= tol)
      return true;
    x -= fx / f.differentiate(x);
  }
  return false;
}

// Explicit Instantiations
template bool newton_solve(double &x, const polynomial<double> &f, int max_it,
                           double tol);

template bool newton_solve(std::complex<double> &x,
                           const polynomial<std::complex<double> > &f,
                           int max_it, double tol);

template bool newton_solve(vector2<double> &x, const polynomial<double> &f,
                           int max_it, double tol);
}
