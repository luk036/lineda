#include <cmath>
#include <iostream>
using namespace std; // workaround mtl problem
#include <cassert>
#include "ellipsoid.hpp"
#include <mtl/linalg_vec.h>
#include <mtl/matrix.h>
#include <mtl/mtl.h>
#include <mtl/utils.h>
#include <quadratic.hpp>

using namespace mtl;

int main() {
  typedef matrix<double, rectangle<>, dense<>, column_major>::type Matrix;
  typedef dense1D<double> Vec;

  Vec x(3, 0);
  ex_quadratic<Matrix, Vec> lf;

  //  Use Ellipsoid method
  Matrix Ae(3, 3);
  set_diagonal(Ae, 100.);
  int flag = ellipsoid(lf, Ae, x, 200, 1e-8);
  cout << "flag = " << flag << endl;
  print_vector(x); // should be close to [1 3 5]
}
