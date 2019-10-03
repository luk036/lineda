#include "coord_sys.hpp"
#include <iostream>
using namespace std;
using namespace recti;

int main() {
  coord_sys cs1;
  point p1(10, 20);
  point p2(10, 20);
  cout << "original coordinate" << p1 << endl;

  // cs1.rotate180();
  // cs1.translate(10,30);

  // cs1.rotate180();
  // cs1.translate(2,4);

  cs1.rotate090();
  cs1.translate(6, 7);

  cs1.rotate180();
  cs1.translate(2, 4);

  cs1.rotate090();
  // cs1.translate(25,40);

  cs1.xfrom_to(p1);
  // (7, 24) -> rotate090 -> translate(6,7) -> rotate180() -> translate(2,4) ->
  // rotate090() -> p1
  cs1.xfrom_from(p2);
  // p2 -> rotate090 -> translate(6,7) -> rotate180() -> translate(2,4) ->
  // rotate090() -> (13, 16)
  cout << "after transform coordinate" << p1 << endl;
  cout << "after transform coordinate" << p2 << endl;
}
