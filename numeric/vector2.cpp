#include "vector2.hpp"
#include <cmath>

using namespace std;

namespace numeric {
/** Solve quadratic polynomial equation x^2 - v[1]*x - v[2] = 0 */
template <typename Tp>
ostream &display_roots(ostream &out, const vector2<Tp> &v) {
  const Tp a = v._a1 / 2.0;
  const Tp D = a * a + v._a2;
  if (D > 0.0) {
    const Tp d = sqrt(D);
    const Tp x1 = (a > 0) ? a + d : a - d;
    out << x1 << ", ";
    out << -v._a2 / x1;
  } else if (D < 0.0) { // complex roots
    const Tp d = sqrt(-D);
    out << a << " + " << d << "*i, ";
    out << a << " - " << d << "*i";
  } else { // D == 0, double roots
    out << a << ", " << a;
  }
  return out;
}

/** Explicit instantiations */
template struct vector2<int>;

template ostream &operator<<(ostream &out, const vector2<double> &v);
template ostream &operator<<(ostream &out, const vector2<vector2<double> > &v);

template ostream &display_roots(ostream &out, const vector2<double> &v);

template ostream &display_roots(ostream &out, const vector2<float> &v);
}
