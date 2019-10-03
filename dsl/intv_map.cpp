#include "intv_map.hpp"
#include "pst_tree.hpp"
#include <iostream>
#include <string>

using namespace std;

static void dummy() {
  std::intv_map<int, int> S;
  S.insert(make_pair(3, 345), 4);
}
