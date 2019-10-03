#include <aa.h>
#include <ellipsoid.hpp>
#include <ex_sphere.hpp>
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
  double xx[] = {1.0, 1.0, 1.0};
  // Vec x(NUM_VARS);
  std::valarray<AAF> x(NUM_VARS);

  // Make the design variables W's also be variated.
  // for (size_t i=0; i<6; ++i) {
  //   x[i] = log(AAF(interval(-1e-7, 1e-7)) + xx[i]);
  // }

  AAF v = interval(0.9, 1.1);
  // The remaining variables are fixed.
  // xxx for (size_t j=0; j<NUM_VARS; ++j) {
  // xxx   x[j] = log(xx[j] * *(new interval(0.9,1.1)));
  // xxx }
  x[0] = log(xx[0] * v);
  x[1] = log(xx[1] / v);
  x[2] = log(xx[2] / v);

  // xxx ex_sphere<double> lf(x);
  ex_sphere<AAF> lf(x);

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

  // xxx ex_sphere<double> lf2(x);
  ex_sphere<AAF> lf2(x);

  for (size_t i = 0; i < NUM_VARS; ++i) {
    x[i] = exp(x[i]);
  }

  cout << " vx =  " << x[vx].convert() << "\n"
       << " vy =  " << x[vy].convert() << "\n"
       << " vz =  " << x[vz].convert() << "\n"
       << endl;

  cout << (x[vx] * x[vy] * x[vy] * x[vz] * x[vz] * x[vz]).convert() << endl;
}
