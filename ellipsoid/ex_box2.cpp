#include "ex_box2.hpp"
#include <cassert>
#include <cmath>
#include "posynomial.hpp"
#include <valarray>
#include <vector>

using namespace std;

// Problem setup
ex_box2::ex_box2() : _P(7) {
  //----------------------------------------------------------------------
  //      PROBLEM DATA IN POSYNOMIAL FORM (DENSE FORMAT)
  //----------------------------------------------------------------------
  const double Aflr = 1000;
  const double Awall = 100;
  const double alpha = 0.5;
  const double beta = 2;
  const double gamma = 0.5;
  const double delta = 2;

  _P[0] = new posynomial<double>(3, 1);
  _P[0]->_M[0]._a[0] = -1;
  _P[0]->_M[0]._a[1] = -1;
  _P[0]->_M[0]._a[2] = -1;
  _P[0]->_M[0]._b = 0;

  _P[1] = new posynomial<double>(3, 2);
  _P[1]->_M[0]._a[0] = 1;
  _P[1]->_M[0]._a[1] = 1;
  _P[1]->_M[0]._a[2] = 0;
  _P[1]->_M[1]._a[0] = 1;
  _P[1]->_M[1]._a[1] = 0;
  _P[1]->_M[1]._a[2] = 1;
  _P[1]->_M[0]._b = _P[1]->_M[1]._b = log(2. / Awall);

  _P[2] = new posynomial<double>(3, 1);
  _P[2]->_M[0]._a[0] = 0;
  _P[2]->_M[0]._a[1] = 1;
  _P[2]->_M[0]._a[2] = 1;
  _P[2]->_M[0]._b = log(1. / Aflr);

  _P[3] = new posynomial<double>(3, 1);
  _P[3]->_M[0]._a[0] = -1;
  _P[3]->_M[0]._a[1] = 1;
  _P[3]->_M[0]._a[2] = 0;
  _P[3]->_M[0]._b = log(alpha);

  _P[4] = new posynomial<double>(3, 1);
  _P[4]->_M[0]._a[0] = 1;
  _P[4]->_M[0]._a[1] = -1;
  _P[4]->_M[0]._a[2] = 0;
  _P[4]->_M[0]._b = log(1. / beta);

  _P[5] = new posynomial<double>(3, 1);
  _P[5]->_M[0]._a[0] = 0;
  _P[5]->_M[0]._a[1] = 1;
  _P[5]->_M[0]._a[2] = -1;
  _P[5]->_M[0]._b = log(gamma);

  _P[6] = new posynomial<double>(3, 1);
  _P[6]->_M[0]._a[0] = 0;
  _P[6]->_M[0]._a[1] = -1;
  _P[6]->_M[0]._a[2] = 1;
  _P[6]->_M[0]._b = log(1 / delta);
}

ex_box2::~ex_box2() {
  for (size_t i = 0; i < _P.size(); ++i)
    delete _P[i];
}

// Calculator fj(x)
double ex_box2::operator()(const Vec &x, int j) { return (*_P[j])(x); }

// Calculate the gradient vector of fj(x)
void ex_box2::gradient(Vec &g, const Vec &x, int j) { _P[j]->gradient(g, x); }
