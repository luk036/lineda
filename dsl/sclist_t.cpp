#include <cstdlib>
#include <iostream>
#include "sclist.hpp"

using dsl::sclist;

inline int distance(int a, int b) { return abs(a - b); }

int main() {
  sclist<int> scl(7);

  scl.josephus_permutate(3);
  std::cout << scl;
}
