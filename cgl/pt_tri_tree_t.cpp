#include <iostream>
#include "pt_tri_tree.hpp"
#include <svgstream.hpp>

using cgl::pt_tri_tree;
// xxx using dsl::tri_tree_base;

int main() {
  pt_tri_tree T(100);
  T.rmst(3);

  lineda::svgstream test("pt_tri_tree.svg");
  test << T;

  T.print_tree(std::cout);
}
