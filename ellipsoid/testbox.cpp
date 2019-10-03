#include "ellipsoid.hpp"
#include "ex_box2.hpp"
#include <iostream>
#include <valarray>

using namespace std;

int main() {
  typedef std::valarray<double> Vec;
  typedef std::valarray<Vec> Matrix;

  Vec x(0., 3);
  Matrix Ae(Vec(0., 3), 3);
  Ae[0][0] = Ae[1][1] = Ae[2][2] = 100; // set diagonal

  ex_box2 lf;
  int flag = ellipsoid(lf, Ae, x, 500, 1e-8);
  cout << "flag = " << flag << endl;
  cout << "vol_cvx = " << std::exp(x.sum()) << endl;
}
