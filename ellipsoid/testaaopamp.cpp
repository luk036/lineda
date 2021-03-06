#include "ellipsoid.hpp"
#include "ex_opamp2.hpp"
#include <aa.h>
#include <iostream>
#include <valarray>

using namespace std;

int main() {
  typedef std::valarray<double> Vec;
  typedef std::valarray<Vec> Matrix;

  Matrix Ae(Vec(0., NUM_VARS), NUM_VARS);
  for (size_t i = 0; i < NUM_VARS; ++i) {
    Ae[i][i] = 20; // initial radius
  }

  // Initial value
  double xx[] = {832.8e-6, 143.6e-6, 64.6e-6, 588.8e-6, 132.6e-6,
                 2e-6,     0.8e-6,   0.8e-6,  0.8e-6,   0.8e-6,
                 3.5e-12,  10e-6,    0.3,     0.3,      0.3};
  // Vec x(NUM_VARS);
  std::valarray<AAF> x(NUM_VARS);

  // The remaining variables are fixed.
  for (size_t j = 0; j < 6; ++j) {
    x[j] = log(xx[j]);
  }
  for (size_t j = 6; j < NUM_VARS; ++j) {
    x[j] = log(xx[j]);
  }

  ex_opamp2<AAF> lf(x);
  // ex_opamp2<double> lf(x);

  int flag = ellipsoid(lf, Ae, x, 5000, 1e-4);
  cout << "flag = " << flag << endl;
  switch (flag) {
  case 0:
    cout << "solution found to tolerance\n\n";
    break;
  case 1:
    cout << "no convergence given max_it\n\n";
    break;
  case 2:
    cout << "no feasible sol'n\n\n";
    break;
  case 3:
    cout << "dot product becomes negative\n\n";
    break;
  default:
    break;
  }

  ex_opamp2<AAF> lf2(x);

  for (size_t i = 0; i < NUM_VARS; ++i) {
    x[i] = exp(x[i]);
  }

  cout << "  \\begin{tabular}{|l|c|l|}\n"
       << "    \\hline\n"
       << "    Variable          & Units    & Value \\\\ \n"
       << "    \\hline\\hline\n"
       << "    $W_1 = W_2$       & $\\mu$m   & $" << (x[W1] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $W_3 = W_4$       & $\\mu$m   & $" << (x[W3] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $W_5$             & $\\mu$m   & $" << (x[W5] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $W_6$             & $\\mu$m   & $" << (x[W6] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $W_7$             & $\\mu$m   & $" << (x[W7] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $W_8$             & $\\mu$m   & $" << (x[W8] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $L_1 = L_2$       & $\\mu$m   & $" << (x[L1] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $L_3 = L_4$       & $\\mu$m   & $" << (x[L3] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $L_5 = L_7 = L_8$ & $\\mu$m   & $" << (x[L5] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $L_6$             & $\\mu$m   & $" << (x[L6] * 1e6).convert()
       << "$ \\\\ \n"
       << "    $C_c$             & pF        & $" << (x[Cc] * 1e12).convert()
       << "$ \\\\ \n"
       << "    $I_{bias}$        & $\\mu$A   & $" << (x[Ibias] * 1e6).convert()
       << "$ \\\\ \n"
       << "    \\hline\n"
       << "  \\end{tabular}\n";

  cout << endl;
}
