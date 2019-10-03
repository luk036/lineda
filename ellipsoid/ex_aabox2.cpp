#include "ex_aabox2.hpp"
#include <aa.h>
#include <cassert>
#include <cmath>
#include <posynomial.hpp>
#include <valarray>
#include <vector>

using namespace std;

// Problem setup
ex_aabox2::ex_aabox2() : _P(7) {
  //----------------------------------------------------------------------
  //      PROBLEM DATA IN POSYNOMIAL FORM (DENSE FORMAT)
  //----------------------------------------------------------------------
  const AAF Aflr(interval(999.9, 1000.1));
  const AAF Awall(interval(99.9, 100.1));
  const double alpha = 0.5;
  const double beta = 2;
  const double gamma = 0.5;
  const double delta = 2;

  _P[0] = new posynomial<AAF>(3, 1);
  _P[0]->_M[0]._a[0] = -1;
  _P[0]->_M[0]._a[1] = -1;
  _P[0]->_M[0]._a[2] = -1;
  _P[0]->_M[0]._b = 0;

  _P[1] = new posynomial<AAF>(3, 2);
  _P[1]->_M[0]._a[0] = 1;
  _P[1]->_M[0]._a[1] = 1;
  _P[1]->_M[0]._a[2] = 0;
  _P[1]->_M[1]._a[0] = 1;
  _P[1]->_M[1]._a[1] = 0;
  _P[1]->_M[1]._a[2] = 1;
  _P[1]->_M[0]._b = _P[1]->_M[1]._b = log(2.) - log(Awall);

  _P[2] = new posynomial<AAF>(3, 1);
  _P[2]->_M[0]._a[0] = 0;
  _P[2]->_M[0]._a[1] = 1;
  _P[2]->_M[0]._a[2] = 1;
  _P[2]->_M[0]._b = -log(Aflr);

  _P[3] = new posynomial<AAF>(3, 1);
  _P[3]->_M[0]._a[0] = -1;
  _P[3]->_M[0]._a[1] = 1;
  _P[3]->_M[0]._a[2] = 0;
  _P[3]->_M[0]._b = log(alpha);

  _P[4] = new posynomial<AAF>(3, 1);
  _P[4]->_M[0]._a[0] = 1;
  _P[4]->_M[0]._a[1] = -1;
  _P[4]->_M[0]._a[2] = 0;
  _P[4]->_M[0]._b = log(1. / beta);

  _P[5] = new posynomial<AAF>(3, 1);
  _P[5]->_M[0]._a[0] = 0;
  _P[5]->_M[0]._a[1] = 1;
  _P[5]->_M[0]._a[2] = -1;
  _P[5]->_M[0]._b = log(gamma);

  _P[6] = new posynomial<AAF>(3, 1);
  _P[6]->_M[0]._a[0] = 0;
  _P[6]->_M[0]._a[1] = -1;
  _P[6]->_M[0]._a[2] = 1;
  _P[6]->_M[0]._b = log(1 / delta);
}

ex_aabox2::~ex_aabox2() {
  for (size_t i = 0; i < _P.size(); ++i)
    delete _P[i];
}

// Calculator fj(x)
double ex_aabox2::operator()(const Vec &x, int j) { return (*_P[j])(x); }

// Calculate the gradient vector of fj(x)
void ex_aabox2::gradient(Vec &g, const Vec &x, int j) { _P[j]->gradient(g, x); }
