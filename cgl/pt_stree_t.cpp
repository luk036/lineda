#include <iostream>
#include "pt_stree.hpp"
#include <svgstream.hpp>

using cgl::pt_stree;
// xxx using dsl::stree_base;

int main() {
  pt_stree T(100);
  T.rmst();

  svgstream test("pt_stree.svg");
  test << T;

  T.print_tree(std::cout);
}
