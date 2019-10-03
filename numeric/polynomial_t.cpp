#include "matrix2.hpp"
#include "newton.hpp"
#include "polynomial.hpp"
#include "vector2.hpp"
#include <fstream>
#include <iostream>

using namespace std;

int main() {
  const double x1[] = {1.0, 2.0, 5.0, 7.0, 9.0};
  const double y1[] = {-1.0, 3.0, 4.0, 8.0, 4.0};
  const double x2[] = {2.0, 5.0, 1.0, 7.0, 9.0};  // same set but in
  const double y2[] = {3.0, 4.0, -1.0, 8.0, 4.0}; // different order

  numeric::polynomial<double> poly1(x1, y1, 5);
  numeric::polynomial<double> poly2(x2, y2, 5);

  double y = poly1(5.0); // evaluate at x=5.0
  cout << y << endl;     // expect y=4.0

  // Write data to a file for plotting
  ofstream out("test_poly1.dat");
  out << "# Test data for gnuplot\n";
  for (double t = 0.0; t < 10.0; t += 0.1) {
    out << t << "\t" << poly1(t) << "\n";
  }
  out.close();

  ofstream out2("test_poly2.dat");
  out << "# Test data for gnuplot\n";
  for (double t = 0.0; t < 10.0; t += 0.1) {
    out2 << t << "\t" << poly2(t) << "\n";
  }
  out2.close();

  bool found;

  double x = 1.16;
  found = newton_solve(x, poly1, 100, 1e-6);

  if (found) {
    cout << "x = " << x;
  } else {
    cout << "Can't find a root";
  }
  cout << endl;

  // Bairstow's method in Newton form
  const double x3[] = {1.0, 3.0, 3.0, 1.0};
  numeric::polynomial<double> poly3(x3, 4);
  vector<double> w = poly3(vector<double>(0, 0));
  cout << w.first << ", " << w.second << endl;
  numeric::matrix2<double> m = poly3.differentiate(vector<double>(0, 0));
  cout << m._a11 << ", " << m._a12 << endl;
  cout << m._a21 << ", " << m._a22 << endl;

  vector<double> v(-2, -1.001);
  found = newton_solve(v, poly3, 100, 1e-6);
  if (found) {
    numeric::display_roots(cout, v);
  } else {
    cout << "Can't find a root";
  }
  cout << endl;

  return 0; // to make VC++ happy
}
