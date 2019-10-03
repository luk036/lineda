#include "path.hpp"
#include "point.hpp"
#include "rpolygon.hpp"
#include "svgstream.hpp"
#include <ctime>

using namespace recti;

int main() {
  srand((unsigned)time(NULL));
  svgstream test("path.svg");

  path pa = path::random(10);
  test << pa << "\n";
  std::cout << pa.area() << "\n";

  rpolygon *rp = pa.create_outline();
  test << *rp << "\n";
  rp->normalize();
  std::cout << rp->area() << "\n";
  delete rp;
}
