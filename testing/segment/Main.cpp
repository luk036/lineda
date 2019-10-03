#include <iostream>
#include <rectangle.hpp>
#include <segment.hpp>

using namespace recti;
using namespace std;

int main() {
  const segment s1(Point<int>(1, 2), Point<int>(6, 7));
  const segment s2(Point<int>(1, 6), Point<int>(7, 2));

  if (s1.overlap(s2)) {
    cout << "OK" << endl;
  } else {
    cout << "Fail" << endl;
  }

  const rectangle r(Point<int>(4, -2), 6, 4);
  if (s1.overlap(r))
    cout << "Fail" << endl;
  else
    cout << "OK" << endl;
}
