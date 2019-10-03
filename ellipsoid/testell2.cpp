#include <cmath>
#include <iostream>
using namespace std; // workaround mtl problem
#include "ellipsoid.hpp"
#include "linfrac.hpp"
#include <mtl/linalg_vec.h>
#include <mtl/matrix.h>
#include <mtl/mtl.h>
#include <mtl/utils.h>

using namespace mtl;

int main() {
  typedef matrix<double, rectangle<>, dense<>, column_major>::type Matrix;
  typedef dense1D<double> Vec;

  // initialize E(A,x)
  Vec x(3, 50.0);

  ex_linfrac<Vec> lf;
  Matrix Ae(3, 3);
  set_diagonal(Ae, 100.);

  //  Use Ellipsoid method
  int flag = ellipsoid(lf, Ae, x, 200, 1e-6);
  cout << "flag = " << flag << endl;
  print_vector(x);
}
