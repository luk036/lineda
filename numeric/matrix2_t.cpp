#include "matrix2.hpp"
#include "vector2.hpp"
#include <iostream>

using namespace std;

int main() {
  numeric::matrix2<double> M(1, 2, 3, 4);
  numeric::vector2<double> v(2, 3);

  v = M * v;
  v = v / M;

  cout << v << endl;
  cout << M._a11 << endl;

  display_roots(cout, numeric::vector2<double>(1000000.00001, 10));
  cout << endl;
}
