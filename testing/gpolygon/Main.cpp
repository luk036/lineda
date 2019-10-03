#include <grpolygon.hpp>
#include <iostream>
#include <rectangle.hpp>
#include <segment.hpp>
#include <svgstream.hpp>

using namespace recti;
using namespace std;

int main() {
  const GRPolygon<int> P(3000);
  svgstream s("grpolygon.svg");
  s << P << '\n';
}
