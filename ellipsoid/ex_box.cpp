#include "ex_box.hpp"
#include <cassert>
#include <cmath>
#include <valarray>
#include <vector>

using namespace std;

// Problem setup
ex_box::ex_box() : _M(6), _inds(8), _A(8, Vec(3)), _b(8), _y(2) {
  const size_t num_terms = 8;

  //----------------------------------------------------------------------
  //      PROBLEM DATA IN POSYNOMIAL FORM (DENSE FORMAT)
  //----------------------------------------------------------------------
  const double Aflr = 1000;
  const double Awall = 100;
  const double alpha = 0.5;
  const double beta = 2;
  const double gamma = 0.5;
  const double delta = 2;

  size_t inds[] = {0, 1, 3, 4, 5, 6, 7, 8};
  _inds = std::vector<size_t>(inds, inds + num_terms);

  _A[0][0] = -1;
  _A[0][1] = -1;
  _A[0][2] = -1;
  _A[1][0] = 1;
  _A[1][1] = 1;
  _A[1][2] = 0;
  _A[2][0] = 1;
  _A[2][1] = 0;
  _A[2][2] = 1;
  _A[3][0] = 0;
  _A[3][1] = 1;
  _A[3][2] = 1;
  _A[4][0] = -1;
  _A[4][1] = 1;
  _A[4][2] = 0;
  _A[5][0] = 1;
  _A[5][1] = -1;
  _A[5][2] = 0;
  _A[6][0] = 0;
  _A[6][1] = 1;
  _A[6][2] = -1;
  _A[7][0] = 0;
  _A[7][1] = -1;
  _A[7][2] = 1;

  double b[] = {1.,    2. / Awall, 2. / Awall, 1. / Aflr,
                alpha, 1. / beta,  gamma,      1. / delta};
  _b = Vec(b, num_terms);

  for (size_t i = 0; i < num_terms; ++i) {
    _b[i] = log(_b[i]);
  }
}

// Calculator fj(x)
double ex_box::operator()(const Vec &x, int j) {
  const size_t N = _inds[j + 1] - _inds[j];
  const size_t n = x.size();

  // y <- b + A*x
  _y.resize(N);
  for (size_t i = 0, k = _inds[j]; i < N; ++i, ++k) {
    _y[i] = _b[k];
    for (size_t j = 0; j < n; ++j) {
      _y[i] += _A[k][j] * x[j];
    }
  }

  if (N == 1) { // monomial
    return _y[0];
  }

  // f <- log(sum_i(exp(y_i)))
  for (size_t i = 0; i < N; ++i) {
    _y[i] = exp(_y[i]);
  }

  return log(_y.sum());
}

// Calculate the gradient vector of fj(x)
void ex_box::gradient(Vec &g, const Vec &x, int j) {
  const size_t N = _inds[j + 1] - _inds[j];
  const size_t n = x.size();

  if (N == 1) { // monomial
    const size_t k = _inds[j];
    for (size_t i = 0; i < n; ++i) {
      g[i] = _A[k][i];
    }
    return;
  }

  // g = Aj' * (exp(yj)./sum(exp(yj)));
  // Note that exp(yj) has been previous calculated in _y during the
  // function evaluation.
  double sum = _y.sum();
  _y /= sum;

  for (size_t i = 0; i < n; ++i) {
    g[i] = 0.;
    for (size_t l = 0, k = _inds[j]; k < _inds[j + 1]; ++k, ++l) {
      g[i] += _A[k][i] * _y[l];
    }
  }
}
