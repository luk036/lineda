#include <aa.h>
#include <iostream>
#include <svgstream.hpp>
#include <zonotope.hpp>

using namespace recti;
using namespace std;

int main() {
  srand((unsigned)time(NULL));

  svgstream sout("zonotope.svg");

  Zonotope<int> w = zonotope::random();
  Zonotope<double> ww = w;
  Zonotope<int> z = zonotope::random();
  Zonotope<double> zz = z;
  sout << zz.approximate(ww) << endl;
  sout << w << endl;
}
