#include "cslink.hpp"
#include "slist.hpp"
#include <iostream>

using namespace dsl;
using namespace std;

int main() {
  cslinks_base a(7);
  cout << a << endl;
  a.josephus_permutate(3);
  cout << a << endl;
}
