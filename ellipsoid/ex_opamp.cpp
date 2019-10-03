#include "ex_opamp.hpp"
#include <cmath>
#include <vector>
#include <valarray>
#include <cassert>

using namespace std;

enum { W1, W3, W5, W6, W7, W8, L1, L3, L5, L6, Cc, Ibias, 
       NUM_VARS, W2=W1, W4=W3, L2=L1, L4=L3, L7=L5, L8=L5 };

// Problem setup
ex_opamp::ex_opamp() 
    : _M(20), _inds(22), _A(80, Vec(0.,NUM_VARS)), _b(80), _y(10) {

  const double
    Vdd = 5.,
    Vss = 0.,
    Vcmmax = 3.,
    Vcmmin = 3.,
    Voutmin = 0.1*Vdd,
    Voutmax = 0.9*Vdd,
    VTN = 0.7,            // NMOS threshold voltage
    VTP = -0.8,           // PMOS threshold voltage
    ln = 103.59e-6,       // channel-length modulation parameter
    lp = 34.53e-6,        // channel-length modulation parameter
    Cox = 3.453e-4,       // oxide capacitance per unit area
    un = ln/Cox,          // electronic mobility
    up = lp/Cox,          // electronic mobility
    Lmax = 0.8e-6,        // max device length (um)
    Lmin = 0.8e-6,        // min device length (um)
    Wmax = 1000e-6,       // max device width  (um)
    Wmin = 2e-6,          // min device width  (um) ???
    Amax = 10000e-12,     // maximum area
    alpha0 = 100.,        // ??? 
    alpha1 = 1000.,       // ???
    alpha2 = 8.,          // ???

    end=1000;



  // III.B. Limits on Device Sizes
  _A[ 1][L1] = 1.;   _b[ 1] = 1./Lmax;  _inds[ 1] =  1;
  _A[ 2][L1] = -1.;  _b[ 2] = Lmin;     _inds[ 2] =  2;
  _A[ 3][L3] = 1.;   _b[ 3] = 1./Lmax;  _inds[ 3] =  3;
  _A[ 4][L3] = -1.;  _b[ 4] = Lmin;     _inds[ 4] =  4;
  _A[ 5][L5] = 1.;   _b[ 5] = 1./Lmax;  _inds[ 5] =  5;
  _A[ 6][L5] = -1.;  _b[ 6] = Lmin;     _inds[ 6] =  6;
  _A[ 7][L6] = 1.;   _b[ 7] = 1./Lmax;  _inds[ 7] =  7;
  _A[ 8][L6] = -1.;  _b[ 8] = Lmin;     _inds[ 8] =  8;
  _A[ 9][W1] = 1.;   _b[ 9] = 1./Wmax;  _inds[ 9] =  9;
  _A[10][W1] = -1.;  _b[10] = Wmin;     _inds[10] = 10;
  _A[11][W3] = 1.;   _b[11] = 1./Wmax;  _inds[11] = 11;
  _A[12][W3] = -1.;  _b[12] = Wmin;     _inds[12] = 12;
  _A[13][W5] = 1.;   _b[13] = 1./Wmax;  _inds[13] = 13;
  _A[14][W5] = -1.;  _b[14] = Wmin;     _inds[14] = 14;
  _A[15][W6] = 1.;   _b[15] = 1./Wmax;  _inds[15] = 15;
  _A[16][W6] = -1.;  _b[16] = Wmin;     _inds[16] = 16;

  // III.C. Area
  _inds[17] = 17;
  _b[17] = alpha0/Amax;
  _A[18][Cc] = 1;  _b[18] = alpha1/Amax;
  _A[19][W1] = _A[19][L1] = 1; _b[19] = alpha2/Amax;
  _A[20][W3] = _A[20][L3] = 1; _b[20] = alpha2/Amax;
  _A[21][W5] = _A[21][L5] = 1; _b[21] = alpha2/Amax;
  _A[22][W6] = _A[22][L6] = 1; _b[22] = alpha2/Amax;
  _A[23][W7] = _A[23][L7] = 1; _b[23] = alpha2/Amax;
  _A[24][W8] = _A[24][L8] = 1; _b[24] = alpha2/Amax;

  // III.D. Systematic Input Offset Voltage
  // Eq.(9) equality constraints handled by two inequality constraints
  _inds[18] = 25;
  _A[25][W3] = _A[25][L6] = _A[25][W7] = _A[25][L5] = 1; 
  _A[25][L3] = _A[25][W6] = _A[25][L7] = _A[25][W5] = -1; 
  _b[25] = 2.;
  _inds[19] = 26;
  _A[26][W3] = _A[26][L6] = _A[26][W7] = _A[26][L5] = -1; 
  _A[26][L3] = _A[26][W6] = _A[26][L7] = _A[26][W5] = 1; 
  _b[26] = 0.5;

  // IV BIAS, CONDITIONS, SIGNAL SWING, AND POWER CONSTRAINTS
  // Eq. (10)
  Vec I5(0.,NUM_VARS);
  I5[W5] = I5[L8] = I5[Ibias] = 1; 
  I5[L5] = I5[W8] = -1;
  Vec I7(0.,NUM_VARS);
  I7[W7] = I5[L8] = I5[Ibias] = 1; 
  I7[L7] = I5[W8] = -1;
}


// Calculator fj(x)
double ex_opamp::operator()(const Vec& x, int j) {
  const size_t N = _inds[j+1] - _inds[j];
  const size_t n = x.size();

  // y <- b + A*x
  _y.resize(N);
  for (size_t i=0, k=_inds[j]; i<N; ++i, ++k) {
    _y[i] = _b[k];
    for (size_t j=0; j<n; ++j) {
      _y[i] += _A[k][j] * x[j];
    }
  }

  if (N == 1) { // monomial
    return _y[0];
  }

  // f <- log(sum_i(exp(y_i)))
  for (size_t i=0; i<N; ++i) {
    _y[i] = exp(_y[i]);
  }

  const double sum = _y.sum();
  return log(sum);
}


// Calculate the gradient vector of fj(x)
void ex_opamp::gradient(Vec& g, const Vec& x, int j) {
  const size_t N = _inds[j+1] - _inds[j];
  const size_t n = x.size();

  if (N == 1) {  // monomial
    const size_t k = _inds[j];
    for (size_t i=0; i<n; ++i) {
      g[i] = _A[k][i];
    }
    return;
  }

  // g = Aj' * (exp(yj)./sum(exp(yj)));
  // Note that exp(yj) has been previous calculated in _y during the
  // function evaluation.
  double sum = _y.sum();
  _y /= sum;

  for (size_t i=0; i<n; ++i) {
    g[i] = 0.;
    for (size_t l=0, k=_inds[j]; k<_inds[j+1]; ++k, ++l) {
      g[i] += _A[k][i] * _y[l];
    }
  }
}

