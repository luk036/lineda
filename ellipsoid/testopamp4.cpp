#include <aa.h>
#include <ellipsoid.hpp>
#include <ex_opamp4.hpp>
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
  double xx[] = {1.0e-6, 3.0e-5, 1.0e-6, 5.0e-6, 1.0e-6, 2.0e-6,
                 1.0e-6, 2.0e-6, 5.0e-6, 20.0,   100.0};
  // Vec x(NUM_VARS);
  std::valarray<double> x(NUM_VARS);

  // Make the design variables W's also be variated.
  // for (size_t i=0; i<6; ++i) {
  //   x[i] = log(AAF(interval(-1e-7, 1e-7)) + xx[i]);
  // }

  // The remaining variables are fixed.
  for (size_t j = 0; j < NUM_VARS; ++j) {
    x[j] = log(xx[j]);
  }

  // xxx ex_opamp4<double> lf(x);
  ex_opamp4<AAF> lf(x);

  int flag = ellipsoid(lf, Ae, x, 5000, 1e-3);
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

  // xxx ex_opamp4<double> lf2(x);
  ex_opamp4<AAF> lf2(x);

  for (size_t i = 0; i < NUM_VARS; ++i) {
    x[i] = exp(x[i]);
  }

  cout << "    W01 =  " << x[vW01] * 1e6 << " um \n"
       << "    W08 =  " << x[vW08] * 1e6 << " um \n"
       << "    W10 =  " << x[vW10] * 1e6 << " um \n"
       << "    W13 =  " << x[vW13] * 1e6 << " um \n"
       << "    L01 =  " << x[vL01] * 1e6 << " um \n"
       << "    L08 =  " << x[vL08] * 1e6 << " um \n"
       << "    L10 =  " << x[vL10] * 1e6 << " um \n"
       << "    L13 =  " << x[vL13] * 1e6 << " um \n"
       << "    Ib  =  " << x[vIbias] * 1e6 << " uA \n"
       << "    A   =  " << x[vA] << "    \n"
       << "    B   =  " << x[vB] << "    \n"
       << endl;

  // xxx cout
  // xxx   << "  \\begin{tabular}{|l|c|l|} \n"
  // xxx   << "    \\hline \n"
  // xxx   << "    Variable          & Units    & Value \\\\ \n"
  // xxx   << "    \\hline\\hline \n"
  // xxx   << "    $W_1 = W_2$       & $\\mu$m   & $" << (x[W1] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $W_3 = W_4$       & $\\mu$m   & $" << (x[W3] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $W_5$             & $\\mu$m   & $" << (x[W5] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $W_6$             & $\\mu$m   & $" << (x[W6] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $W_7$             & $\\mu$m   & $" << (x[W7] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $W_8$             & $\\mu$m   & $" << (x[W8] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $L_1 = L_2$       & $\\mu$m   & $" << (x[L1] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $L_3 = L_4$       & $\\mu$m   & $" << (x[L3] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $L_5 = L_7 = L_8$ & $\\mu$m   & $" << (x[L5] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $L_6$             & $\\mu$m   & $" << (x[L6] * 1e6
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $C_c$             & pF       & $"  << (x[Cc] * 1e12
  // ).convert()  << "$ \\\\ \n"
  // xxx   << "    $I_{bias}$        & $\\mu$A   & $" << (x[Ibias]
  // *1e6).convert()  << "$ \\\\ \n"
  // xxx   << "    \\hline \n"
  // xxx   << "  \\end{tabular} \n";

  cout << endl;
}
