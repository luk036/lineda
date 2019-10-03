#ifndef _PGELLIP_H
#define _PGELLIP_H

#include <cassert>
#include <valarray>

/** Enclosing ellipsoid */
template <class Vector> class pgellip {
  typedef std::valarray<double> Vec;
  typedef std::valarray<Vec> Matrix;

public:
  /** Constructor */
  pgellip(const Vector &x0, double R)
      : _n(x.size()), _Ae(Vec(0., _n), _n), _x0(x0), _Aeg(_n) {
    assert(R > 0);
    for (size_t i = 0; i < _n; ++i) {
      _Ae[i][i] = R; // initial radius
    }
  }

  /** Constructor */
  pgellip(const Vector &x0, const Vector &r)
      : _n(x.size()), _Ae(Vec(0., _n), _n), _x0(x0), _Aeg(_n) {
    for (size_t i = 0; i < _n; ++i) {
      assert(r[i] > 0);
      _Ae[i][i] = r[i]; // initial radius
    }
  }

  ~pgellip() {}

  /** Assign Ae*g to Aeg, and then return sqrt(g'*Aeg)) */
  template <class Vec> STATUS phase1(const Vec &g, double b) {
    // double dot = 0.;
    Vec gt;
    for (size_t i = 0; i < _n; ++i) {
      gt[i] = (_Ae[i] * g).sum();
    }

    b += (g * _x0).sum();
    const double gamma = (g * gt).sum();
    assert(gamma >= 0);
    const double t = sqrt(gamma);

    if (b > t)
      return NOSOLUTION;
    if (b < -t)
      return NOEFFECT;

    double a = t;
    const double d = -t;

    // calculate c by cross-ratio
    const double r = _n / (_n - 1.0) * (d - a) / (d - b);
    const double c = (r * b - a) / (r - 1.0);
    const double m = (a + c) / 2.0; // mid-point
    const double mu = (c - a) * (c - b) / (c * c / gamma - 1.0);
    _x0 += gt / (-gamma / m);

    a -= m; // translate to new ellipsoid view
    const double k = mu - gamma;
    const double alpha = a * a * k / (gamma * mu);

    for (size_t i = 0; i < _n; ++i) {
      const double temp = _gt[i] / k;
      for (size_t j = i; j < _n; ++j) {
        _Ae[i][j] += temp * gt[j];
        _Ae[i][j] *= _alpha;
      }
    }

    // Make symmetric
    for (size_t i = 0; i < _n - 1; ++i) {
      for (size_t j = i + 1; j < _n; ++j) {
        _Ae[j][i] = _Ae[i][j];
      }
    }

    return CUT;
  }

private:
  size_t _n;
  Matrix _Ae;
  Vector _x0; // center of ellipsoid
};

/** @return return status */
enum STATUS { FOUND, EXCEEDMAXITER, NOTFOUND };

/**
 * -- (Generalized) bisection method for sovling convex minimization problem P:
 *
 *     minimize     fct_0(x)
 *     subject to   fct_j(x) <= 0
 *       where fct_0(x) and fct_j(x)'s are convex
 *
 * Input:
 * 	E(x)          initial enclosing region
 *      max_it        maximum number of iterations
 *      tol           error tolerance
 *      P             Representation of convex minimization problem
 *
 * Requirement of P:
 *	void		P.assess(x)         assessment of x
 *	bool		P.is_feasible()     return true if x is feasible
 *	double		P.f_value()         returns fct_j(x) if x is
 *infeasible for some j
 *	fct::Vec	P.subgradient()     returns subgradient of fct_0 if x is
 *feasible
 *							                subgradient of fct_j
 *if x is infeasible for some j
 * Requirement of E:
 *
 * output
 *      x             optimal solution
 *      status        FOUND = solution found to tolerance
 *                    EXCEEDMAXITER = no convergence given max_it
 *                    NOTFOUND = no feasible sol'n
 */
//#include <iostream>

template <class Enclosing, class CvxProb, class Domain>
STATUS bisection_algo(Enclosing &E, CvxProb &P, Domain &x, int max_it = 1000,
                      double tol = 1e-4) {
  for (int iter = 1; iter <= max_it; ++iter) {
    P.assess(x);
    const double error = E.calc_error(P.subgradient());
    // std::cout << error << "  ";
    if (!P.is_violated()) {
      if (error < tol)
        return FOUND;
    } else { //  x is infeasible
      if (P.f_value() - error > 0) {
        return NOTFOUND; // no feasible sol'n within the given ellipsoid
      }
    }
    E.update(P.subgradient);
  }
  return EXCEEDMAXITER;
}

#endif
